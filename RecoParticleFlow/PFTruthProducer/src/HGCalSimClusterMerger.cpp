
#include "../interface/HGCalSimClusterMerger.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

//DEBUG
#include <fstream>

std::vector<SimCluster> HGCalSimClusterMerger::merge(const std::vector<const SimCluster*>& scs,
        float threshold,
        std::vector<std::vector<size_t> >& idxs) const{
    //create the properties


    //initiate merger
    merger::ObjectMerger<CaloSimClusterCircleMergeWrapper,SimCluster> merger;
    //...

    std::vector<double> debugradii;
    for(const auto& sc:scs){
        merger.add(sc);
        auto mobj = merger.getLast();
        float rad = calcCircle(sc);
        mobj->setRadius(rad);

        debugradii.push_back(rad);
    }

    ;
    auto merged = merger.mergeSymmetric(-threshold,&idxs);

    static int counter=0;
    //DEBUG OUTPUT, dumb json
    std::ofstream f("file"+std::to_string(counter)+".dat");
    counter++;
    f << "[";

    int nm=idxs.size();
    for(const auto& ii:idxs){
        f << "[";
        int nc=ii.size();
        for(const auto& i:ii){
            const auto& pos = scs.at(i)->g4Tracks().at(0).getPositionAtBoundary();
            f << "[" <<  pos.x() <<", " << pos.y() << ", " << pos.z() << ", " << debugradii.at(i) << "]";
            if(nc>1)
                f<<", ";
            nc--;
        }
        if(nm>1)
            f << "],";
        else
            f << "]";
        nm--;
    }

    f << "]";
    f.close();
    //
    return merged;
}


double HGCalSimClusterMerger::calcCircle(const SimCluster* sc)const{
    auto hafs = sc->hits_and_fractions();

    //get the hits that match the requirements only

    if(!isHGCal(*sc))
        return -1;

    int firstlayer=100;
    for(const auto haf:hafs){
        const HGCRecHit* rh = getHit(haf.first);
        if(!rh) continue;
        int layer = rechittools_->getLayer(haf.first);
        if(firstlayer > layer)
            firstlayer=layer;
    }

    LocalVector axis(sc->impactMomentum().x(),sc->impactMomentum().y(),sc->impactMomentum().z());
    axis=axis.unit();

    //now get the relevant hits
    std::vector<std::pair<DetId, float> > assignedHafs;
    std::vector<float> hitDistanceToAxis, hitTime;
    std::vector<LocalVector> hitProjAlongAxis;
    double assignedEnSum=0;
    for(const auto haf:hafs){
        const HGCRecHit* rh = getHit(haf.first);
        if(!rh) continue;
        int layer = rechittools_->getLayer(haf.first);
        if(layer >= firstlayer+cNLayers_)
            continue;
        LocalVector hit_pos= getHitPosVec(haf.first);

        auto projection_along_axis = hit_pos.dot(axis) * axis;
        hitProjAlongAxis.push_back(projection_along_axis);
        auto dist_to_axis = hit_pos-projection_along_axis;

        assignedHafs.emplace_back(haf.first,haf.second);
        hitDistanceToAxis.push_back(dist_to_axis.mag());
        hitTime.push_back(rh->time());

        assignedEnSum += rh->energy();
    }

    if(assignedHafs.size()<1){
        return 1e-1;
    }
    // assignedHafs are guaranteed to have rechits

    auto ldsort = argsort(hitTime);//hitDistanceToAxis);
    apply_argsort_in_place(hitDistanceToAxis,ldsort);
    apply_argsort_in_place(hitProjAlongAxis,ldsort);
    apply_argsort_in_place(hitTime,ldsort);
    apply_argsort_in_place(assignedHafs,ldsort);

    auto centralId = assignedHafs.at(0).first;

    if(hitDistanceToAxis.at(0) > cSearchRadius_ * rechittools_->getRadiusToSide(centralId))
        return 1e-1; //not dense hit

    LocalVector centralhitpos = getHitPosVec(centralId);
    double ensum=0;
    double currentradius=0;
    float hitradius = 1e-6;
    for (size_t i = 0; i < assignedHafs.size(); i++) {
        DetId hid = assignedHafs.at(i).first;
       // rechittools_->
        const HGCRecHit* rh = getHit(hid);

        //auto hitpos = getHitPosVec(hid); //use projected positions here
        float dist = (hitProjAlongAxis.at(i) - centralhitpos).mag();//
        if(rechittools_->isSilicon(hid)){
            hitradius = cClusterRadiusScale_ * rechittools_->getRadiusToSide(hid);
        }
        else if(rechittools_->isScintillator(hid)){
            auto etaphi = rechittools_->getScintDEtaDPhi(hid);
            float maxdetadphi = std::max(etaphi.first,etaphi.second);
            hitradius = cClusterRadiusScale_ * 0.5 * centralhitpos.perp() * maxdetadphi;
        }
        else{ //not an hgcal hit
            continue;}
        if(hitradius > 50.)
            continue;//invalid radius

        double rsum = currentradius + hitradius;
        double reldist = dist / (rsum+1e-6);

        if(reldist>1.)
            break;// done

        ensum += rh->energy();
        if(ensum/assignedEnSum > cEContainment_)
            break;

        if(dist > currentradius)
            currentradius=dist;
    }
    currentradius+=hitradius; //add the last hit radius
    if(currentradius>10.)
        std::cout << "large radius " << currentradius << std::endl;//DEBUG
    return currentradius;

}

bool HGCalSimClusterMerger::isHGCal(const SimCluster& cluster)const{

    for (const auto& hitsAndEnergies : cluster.hits_and_fractions()) {
        const DetId id = hitsAndEnergies.first;
        bool forward = id.det() == DetId::HGCalEE
                || id.det() == DetId::HGCalHSi
                || id.det() == DetId::HGCalHSc
                || (id.det() == DetId::Forward && id.subdetId() != static_cast<int>(HFNose))
                || (id.det() == DetId::Hcal && id.subdetId() == HcalSubdetector::HcalEndcap);

        if(forward)
            return true;
    }
    return false;
}

void HGCalSimClusterMerger::createHitMap(){
    hitmap_.clear();
    for(const auto& rh: *rechits_)
        hitmap_[rh.detid()]=&rh;
}

const HGCRecHit* HGCalSimClusterMerger::getHit(DetId id) const {
  auto it = hitmap_.find(id);
  if (it == hitmap_.end())
      return 0;
  return it->second;
}
