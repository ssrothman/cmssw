#include "../interface/CaloSimClusterCircleMergeWrapper.h"
#include <limits>

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

    out.setPdgId(simhist_->createMergedSimClusterID(&out));

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




