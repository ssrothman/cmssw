
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

    // the underlying merger selects *above* threshold, so if
    // we want to merge two SCs with a distance smaller than
    // threshold, both have to be inverted
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

    // restrict to simclusters with at least one hit in the HGCAL
    if(!isHGCal(*sc))
        return -1;

    // fill information for the hits belonging to the simcluster
    // and determine first layer while at it
    std::vector<HitStruct> scHits;
    int firstlayer=100;
    for(const auto haf:hafs){
        const HGCRecHit* rh = getHit(haf.first);
        bool ishgcal = isHGCal(haf.first);
        if(!rh || !ishgcal) continue;

        int layer = rechittools_->getLayer(haf.first);
        if(firstlayer > layer)
            firstlayer=layer;
        float radius = getHitRadius(haf.first);
        scHits.emplace_back(haf.first, rh, haf.second, radius);

    }

    //create the reference point and axis
    LocalVector axis(sc->impactMomentum().x(),sc->impactMomentum().y(),sc->impactMomentum().z());
    axis=axis.unit();

    auto bpos4v= sc->g4Tracks().at(0).getPositionAtBoundary();
    LocalVector boundaryPos(bpos4v.x(),bpos4v.y(),bpos4v.z());


    // store only the hits relevant to calculate the radii
    // also determine their distance to the shower axis
    // and their deposited energy sum
    std::vector<const HitStruct*> assignedHits;
    std::vector<float> hitDistanceToAxis;
    double assignedEnSum=0;
    for(const auto& h: scHits){
        if((int)rechittools_->getLayer(h.id) > cNLayers_)
            continue;

        assignedEnSum += h.energy();
        auto rel_hit_pos = getHitPosVec(h.id) - boundaryPos;
        auto projection_along_axis = rel_hit_pos.dot(axis) * axis;
        float dist_to_axis = (rel_hit_pos-projection_along_axis).mag();

        assignedHits.push_back(&h);
        hitDistanceToAxis.push_back(dist_to_axis);

    }
    if(assignedHits.size()<1)
        return -1;

    auto ldsort = argsort(hitDistanceToAxis);//hitDistanceToAxis);
    apply_argsort_in_place(assignedHits,ldsort);
    apply_argsort_in_place(hitDistanceToAxis,ldsort);

    auto centralHit = assignedHits.at(0);

    if(hitDistanceToAxis.at(0) > cSearchRadius_ * centralHit->radius)
        return 1e-1; //not dense hit

 //   LocalVector centralHitPos = getHitPosVec(centralHit->id);

    double ensum=0;
    double currentradius=0;
    float hitradius = 1e-6;

    for (size_t i = 0; i < assignedHits.size(); i++) {
        auto hit = assignedHits.at(i);

        auto hit_pos = getHitPosVec(hit->id) - boundaryPos;//centralHitPos;//now w.r.t. central hit
        auto projection_along_axis = hit_pos.dot(axis) * axis;
        float dist = (hit_pos-projection_along_axis).mag();

        hitradius = hit->radius;
        double rsum = currentradius + cClusterRadiusScale_ * hitradius;
        double reldist = dist / (rsum+1e-6);

        if(reldist>1.)
            break;// done

        ensum += hit->energy();
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
        if(isHGCal(id))
            return true;
    }
    return false;
}

bool HGCalSimClusterMerger::isHGCal(DetId id)const{
    return id.det() == DetId::HGCalEE
            || id.det() == DetId::HGCalHSi
            || id.det() == DetId::HGCalHSc
            || (id.det() == DetId::Forward && id.subdetId() != static_cast<int>(HFNose))
            || (id.det() == DetId::Hcal && id.subdetId() == HcalSubdetector::HcalEndcap);
}

float HGCalSimClusterMerger::getHitRadius(DetId id)const{
    if(rechittools_->isSilicon(id)){
        return rechittools_->getRadiusToSide(id);
    }
    else if(rechittools_->isScintillator(id)){
        auto hit_pos = getHitPosVec(id);
        auto etaphi = rechittools_->getScintDEtaDPhi(id);
        float maxdetadphi = std::max(etaphi.first,etaphi.second);
        return 0.5 * hit_pos.perp() * maxdetadphi * sqrt(2.);
    }
    return 0;
}


void HGCalSimClusterMerger::createHitMap(){
  hitmap_.clear();
  for (size_t i = 0; i < rechits_->size(); i++)
    hitmap_[(*rechits_)[i].detid()] = i;
}



const HGCRecHit* HGCalSimClusterMerger::getHit(DetId id) const {
  auto it = hitmap_.find(id);
  if (it == hitmap_.end())
      return 0;
  return &(*rechits_)[it->second];
}

