#include "TruthMergerTools.h"

#include "SimDataFormats/CaloAnalysis/interface/SimClusterFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"



class CaloSimClusterCircleMergeWrapper: public merger::MergeWrapper<SimCluster,CaloSimClusterCircleMergeWrapper>{

public:
    enum caloEn {isHgcal,isBarrel}; //etc

    CaloSimClusterCircleMergeWrapper(const SimCluster* x):sc_(x),
    circle_radius_(0){ }

    void setRadius(float r){circle_radius_=r;}

    float mergeScore(const CaloSimClusterCircleMergeWrapper* rhs)const;

    SimCluster mergeAllInList(const std::vector<CaloSimClusterCircleMergeWrapper* >& tomerge)const;

private:

    math::XYZTLorentzVectorF boundaryPos()const;

    const SimCluster* sc_;

    float circle_radius_;

};


