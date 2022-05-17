#include "../interface/CaloSimClusterCircleMergeWrapper.h"
#include <limits>
#include <numeric>

float CaloSimClusterCircleMergeWrapper::mergeScore(const CaloSimClusterCircleMergeWrapper* rhs)const{

    if(circle_radius_<0 || rhs->circle_radius_<0)
        return  -std::numeric_limits<float>::max();//minimum score possible

    auto diff = boundaryPos()-rhs->boundaryPos();
    diff.SetE(0);//set time diff to zero for now

    double dmagsq = diff.x()*diff.x() + diff.y()*diff.y() + diff.z()*diff.z() + diff.t()*diff.t();

    double rsum = circle_radius_+rhs->circle_radius_;
    double score = dmagsq / (rsum*rsum + 1e-9);
    return -sqrt(score);

}

SimCluster CaloSimClusterCircleMergeWrapper::mergeAllInList(
        const std::vector<CaloSimClusterCircleMergeWrapper* >& tomerge)const{
    SimCluster out;

    if(tomerge.size()<1)
        return out;
    if(tomerge.size()==1)
        return *tomerge.at(0)->sc_;

    math::XYZTLorentzVectorF p4, point;
    double totalE=0;
    for(const auto& scw: tomerge){
        out += *(scw->sc_);
    }
    //////// more complex part assigning an ID

    out.setPdgId(createMergedSimClusterID(&out));

    return out;
}

math::XYZTLorentzVectorF CaloSimClusterCircleMergeWrapper::boundaryPos()const{ //this is a workaround until fully filled
    return sc_->g4Tracks().at(0).getPositionAtBoundary();//DEBUG FIXME check with below
    math::XYZTLorentzVectorF p = sc_->impactPoint();
    if(p.mag2()==0){//not filled
        p = sc_->g4Tracks().at(0).getPositionAtBoundary();
    }
    return p;
}


int CaloSimClusterCircleMergeWrapper::createMergedSimClusterID(const SimCluster* sc)const{

    int mergedPdgId=0;
    double idenfrac=0.9;
    double totalE = sc->impactMomentum().E();

    //run on the merged simtracks and their history
    const auto& sts = sc->g4Tracks();



    /*
     * start with the highest energy and check
     * - find the common ancestor that is
     */

    // the following are the more complex cases
    /*
     * std::set_intersection seems like a good choice but actually adds overhead
     * since we know that the simtracks are sorted by energy
     * and only the ones in history with energy > idenfrac*totalE && energy <= totalE
     * can possibly contribute.
     *
     * needs convert from SC->ST 1:1
     * to ST -> SC 1:many
     * then look for best match of E_first ~ sum_E(second) withing bounds, pick best
     */

    std::vector<std::vector<const SimTrack*> > scHists;
    std::vector<const SimTrack*> allst;//unique st pointers
    double ensum=0;
    for(const auto& st: sts){
        scHists.emplace_back(simhist_->createHistory(&st));
        const auto& last = scHists.at(scHists.size()-1);
        for(const auto& lst:last){
            if(std::find(allst.begin(),allst.end(),lst) == allst.end())
                allst.push_back(lst);
        }
    }
    std::vector<float> enIntoGroup(allst.size(),0.);

    for(size_t ip=0;ip<allst.size();ip++){//unique
        for(size_t i=0;i<scHists.size();i++){
            if(std::find(scHists.at(i).begin(),scHists.at(i).end(),allst.at(ip)) != scHists.at(i).end()){
                enIntoGroup.at(ip) += sts.at(i).getMomentumAtBoundary().E();
            }
        }
    }
    //arg sort
    std::vector<std::size_t> sortEnIntoGroup(enIntoGroup.size());
    std::iota(sortEnIntoGroup.begin(), sortEnIntoGroup.end(), 0);
    std::sort(sortEnIntoGroup.begin(), sortEnIntoGroup.end(),
            [&](std::size_t i, std::size_t j){ return enIntoGroup[i] > enIntoGroup[j]; });

    //which has the largest contribution

    mergedPdgId=0;
    for(const auto& idx: sortEnIntoGroup){
        float stE = allst.at(idx)->momentum().E();
        float gE = enIntoGroup.at(idx);
        float motherFracE = gE/stE;

        if(gE/totalE < idenfrac)
            break;

        if(motherFracE > idenfrac){
            mergedPdgId = allst.at(idx)->type();
        }

        std::cout << allst.at(idx)->type() << ": " << allst.at(idx)->momentum().E()
                << "; " << enIntoGroup.at(idx) << " | "<<totalE << "\n";
    }

    // now stToDaughterEnFrac contains for each parent the energy fraction of SCs
    // that are daughters and merged in this group
    // if there is a parent that
    //   (a) has most of the total group's energy
    //   (b) has a good fraction if its energy contained in the group
    //       (stToDaughterEnFrac close to 1)
    // it is impossible (if idenfrac large enough)
    // that a and b are fulfilled simultaneously by more than one ST


    return mergedPdgId;

}


