
#include "../interface/HGCalSimClusterMerger.h"

std::vector<SimCluster> HGCalSimClusterMerger::merge(const std::vector<const SimCluster*>& scs,
        float threshold,
        std::vector<std::vector<size_t> >& idxs) const{
    //create the properties

    //initiate merger
    merger::ObjectMerger<CaloSimClusterCircleMergeWrapper,SimCluster> merger;
    //...
    merger.setThresholdIsUpper(true);//large distances will not get merged

    for(const auto& sc:scs){
        merger.add(sc);
        auto mobj = merger.getLast();
        mobj->setRadius(calcCircle(sc));
    }

    ;
    auto merged = merger.mergeSymmetric(threshold,&idxs);
    return merged;
}


double HGCalSimClusterMerger::calcCircle(const SimCluster* sc)const{
    auto hafs = sc->hits_and_fractions();

    //get the hits that match the requirements only

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
    double assignedEnSum=0;
    for(const auto haf:hafs){
        const HGCRecHit* rh = getHit(haf.first);
        if(!rh) continue;
        int layer = rechittools_->getLayer(haf.first);
        if(layer >= firstlayer+cNLayers_)
            continue;
        LocalVector hit_pos= getHitPosVec(haf.first);

        auto projection_along_axis = hit_pos.dot(axis) * axis;
        auto dist_to_axis = hit_pos-projection_along_axis;

        assignedHafs.emplace_back(haf.first,haf.second);
        hitDistanceToAxis.push_back(dist_to_axis.mag());
        hitTime.push_back(rh->time());

        assignedEnSum += rh->energy();
    }

    if(assignedHafs.size()<1){
        return 0;
    }
    // assignedHafs are guaranteed to have rechits

    auto ldsort = argsort(hitDistanceToAxis);
    apply_argsort_in_place(hitDistanceToAxis,ldsort);
    apply_argsort_in_place(hitTime,ldsort);
    apply_argsort_in_place(assignedHafs,ldsort);

    auto centralId = assignedHafs.at(0).first;


    if(hitDistanceToAxis.at(0) > cSearchRadius_ * rechittools_->getRadiusToSide(centralId))
        return 0; //not dense hit

    //LocalVector centralhitpos = getHitPosVec(centralId);
    double ensum=0;
    double currentradius=hitDistanceToAxis.at(0);

    for (size_t i = 0; i < assignedHafs.size(); i++) {
        DetId hid = assignedHafs.at(i).first;
        const HGCRecHit* rh = getHit(hid);

        //auto hitpos = getHitPosVec(hid); //use projected positions here
        float dist = hitDistanceToAxis.at(i);//(hitpos-centralhitpos).mag();

        float hitradius = cClusterRadiusScale_ * rechittools_->getRadiusToSide(hid);

        double rsum = currentradius + hitradius;
        double reldist = dist / rsum;

        //std::cout << "rsum "<< rsum << " dist " << dist << " hitrad " << hitradius << std::endl;

        if(reldist>1)
            break;// done


        ensum += rh->energy();
        if(ensum/assignedEnSum > cEContainment_)
            break;

        if(dist > currentradius)
            currentradius=dist;
    }
    std::cout <<"radius " << currentradius << std::endl;
    return currentradius;

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
