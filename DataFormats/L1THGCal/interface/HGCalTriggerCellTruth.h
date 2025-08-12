#ifndef DATAFORMATS_L1THGCAL_TRIGGERCELLTRUTH_H
#define DATAFORMATS_L1THGCAL_TRIGGERCELLTRUTH_H

#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/Common/interface/ValueMap.h"

namespace l1t {

class HGCalTriggerCellTruth {
public:
    HGCalTriggerCellTruth () = default;
    HGCalTriggerCellTruth (
                const std::vector<std::pair<int, float>>& simclusters,
                float simenergy, float allocSimEnergyFrac, int nSimCluster)
        : simclusters_(simclusters),
          simenergy_(simenergy), 
          allocSimEnergyFrac_(allocSimEnergyFrac),
          nSimClusters_(nSimCluster) {}

    const std::vector<std::pair<int, float>>& simclusters() const { 
        return simclusters_; 
    }

    float simEnergy() const { 
        return simenergy_; 
    }

    float allocSimEnergyFraction() const { 
        return allocSimEnergyFrac_; 
    }

    int nSimClusters() const { 
        return nSimClusters_; 
    }

protected:
    std::vector<std::pair<int, float>> simclusters_;
    float simenergy_;
    float allocSimEnergyFrac_;
    int nSimClusters_;
};

};

#endif
