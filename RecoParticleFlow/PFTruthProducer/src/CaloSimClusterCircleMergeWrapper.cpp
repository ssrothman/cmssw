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
        out += *(scw->sc_); //this only merges hits, also accounting for fractions
        p4 += scw->sc_->impactMomentum();
        double E = scw->sc_->impactMomentum().E();
        totalE += E;
        point += scw->sc_->impactPoint() * E;//energy weighted
        for(const auto & g4t: scw->sc_->g4Tracks())
            out.addG4Track(g4t);
    }
    point *= 1./totalE;

    out.setImpactMomentum(p4);
    out.setImpactPoint(point);

    /*
     *
     * for the by-primary merger for simclusters:
     *   - energy: simple sum
     *   - ID, exploit that it is by primary:
     *          - take union of all parent simtracks
     *          - determine the one closest in momentum to SC sum
     *          - take that ID if momentum difference within X% threshold
     *          - assign 0 otherwise
     *
     *
     * to retrieve the grouping just make another small wrapper with index (or use the simtrack indices)
     */

    return out;
}
