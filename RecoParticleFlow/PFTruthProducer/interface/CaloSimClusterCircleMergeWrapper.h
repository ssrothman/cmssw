
#ifndef RECOPARTICLEFLOW_PFTRUTHPRODUCER_INTERFACE_CALOSIMCLUSTERCIRCLEMERGEWRAPPER_H_
#define RECOPARTICLEFLOW_PFTRUTHPRODUCER_INTERFACE_CALOSIMCLUSTERCIRCLEMERGEWRAPPER_H_


#include "TruthMergerTools.h"
#include "SimDataFormats/CaloAnalysis/interface/SimClusterFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "SimHistoryTool.h"


class CaloSimClusterCircleMergeWrapper: public merger::MergeWrapper<SimCluster,CaloSimClusterCircleMergeWrapper>{

public:
    enum caloEn {isHgcal,isBarrel}; //etc

    CaloSimClusterCircleMergeWrapper(const SimCluster* x):sc_(x),
    circle_radius_(0),simhist_(0) { }

    void setRadius(float r){circle_radius_=r;}
    void setSimHistoryTool(const SimHistoryTool * st){simhist_=st;}

    float mergeScore(const CaloSimClusterCircleMergeWrapper* rhs)const;

    SimCluster mergeAllInList(const std::vector<CaloSimClusterCircleMergeWrapper* >& tomerge)const;

private:

    math::XYZTLorentzVectorF boundaryPos()const;

    const SimCluster* sc_;

    float circle_radius_;
    //to build the history
    const SimHistoryTool * simhist_;


};

#endif
