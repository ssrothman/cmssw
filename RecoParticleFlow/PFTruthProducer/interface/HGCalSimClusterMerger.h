#include <algorithm>
#include <numeric>

#include "CaloSimClusterCircleMergeWrapper.h"

#include "DataFormats/HGCRecHit/interface/HGCRecHit.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"


//this can be used for PF truth but also for standalone
//in case of PF, it needs to run differently if tracks are present
//therefore, it is more convenient standalone than as an edm::producer
class HGCalSimClusterMerger{
public:
    HGCalSimClusterMerger(const HGCRecHitCollection& rechits,
           const hgcal::RecHitTools * rechittools
    ):rechits_(&rechits),rechittools_(rechittools){
        createHitMap();

        cEContainment_=0.68;
        cSearchRadius_=2.;
        cClusterRadiusScale_=1.;
        cMergeRadiusScale_=1;

    }

    //setters for circle parameters here

    std::vector<SimCluster> merge(const std::vector<const SimCluster*>& scs,
            float threshold,
            std::vector<std::vector<size_t> >& idxs) const;

    void setCClusterRadiusScale(float cClusterRadiusScale) { cClusterRadiusScale_ = cClusterRadiusScale; }

    void setCEContainment(float cEContainment) { cEContainment_ = cEContainment; }

    void setCMergeRadiusScale(float cMergeRadiusScale) { cMergeRadiusScale_ = cMergeRadiusScale; }

    void setCNLayers(int cNLayers) { cNLayers_ = cNLayers; }

    void setCSearchRadius(float cSearchRadius) { cSearchRadius_ = cSearchRadius; }

  private:
    double calcCircle(const SimCluster* sc)const;
    void createHitMap();
    const HGCRecHit* getHit(DetId)const;

    inline LocalVector getHitPosVec(DetId id)const{
        auto gppos = rechittools_->getPosition(id);
        return LocalVector(gppos.x(),gppos.y(),gppos.z());
    }

    //credit to Thomas
    template <typename T>
    std::vector<std::size_t> argsort(const std::vector<T>& vec)const{
        std::vector<std::size_t> p(vec.size());
        std::iota(p.begin(), p.end(), 0);
        std::sort(
                p.begin(), p.end(),
                [&](std::size_t i, std::size_t j){ return vec[i] < vec[j]; }
        );
        return p;
    }

    //credit to Thomas
    template <typename T>
    void apply_argsort_in_place(
            std::vector<T>& vec,
            const std::vector<std::size_t>& p)const
    {
        std::vector<bool> done(vec.size());
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (done[i])
            {
                continue;
            }
            done[i] = true;
            std::size_t prev_j = i;
            std::size_t j = p[i];
            while (i != j)
            {
                std::swap(vec[prev_j], vec[j]);
                done[j] = true;
                prev_j = j;
                j = p[j];
            }
        }
    }

    const HGCRecHitCollection * rechits_;
    const hgcal::RecHitTools* rechittools_ ;
    std::map<DetId, const HGCRecHit*> hitmap_;

    //circle parameters
    int cNLayers_;
    float cEContainment_;
    float cSearchRadius_;
    float cClusterRadiusScale_;
    float cMergeRadiusScale_;

};
