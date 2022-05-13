#include "TruthMergerTools.h"

#include "SimDataFormats/CaloAnalysis/interface/SimClusterFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"



class CaloSimClusterCircleMergeWrapper: public merger::MergeWrapper<SimCluster,CaloSimClusterCircleMergeWrapper>{

public:

    CaloSimClusterCircleMergeWrapper(const SimCluster* x):sc_(x),
    circle_radius_(0){ }

    void setRadius(float r){circle_radius_=r;}

    float mergeScore(const CaloSimClusterCircleMergeWrapper* rhs)const;

    SimCluster mergeAllInList(const std::vector<CaloSimClusterCircleMergeWrapper* >& tomerge)const;

private:

    inline const math::XYZTLorentzVectorF boundaryPos()const{ //this is a workaround until fully filled
        math::XYZTLorentzVectorF p = sc_->impactPoint();
        if(p.mag2()==0){//not filled
            p = sc_->g4Tracks().at(0).getPositionAtBoundary();
        }
        return p;
    }

    const SimCluster* sc_;

    float circle_radius_;
};


