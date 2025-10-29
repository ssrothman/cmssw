#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/CaloTest/interface/HGCalTestNumbering.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalModule.h"
#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/OneToMany.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticleFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCEEDetId.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"


class L1THGCalModuleTruthProducer : public edm::stream::EDProducer<> {
public:
    explicit L1THGCalModuleTruthProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken wafers_token_;
};

L1THGCalModuleTruthProducer::L1THGCalModuleTruthProducer(const edm::ParameterSet& conf)
    : wafers_token_(consumes<edm::View<l1t::HGCalModule>>(conf.getParameter<edm::InputTag>("wafers")))
{

    produces<edm::ValueMap<l1t::HGCalTriggerCellTruth>>();
}

void L1THGCalModuleTruthProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
}

void L1THGCalModuleTruthProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalModule>> wafers_h;
    evt.getByToken(wafers_token_, wafers_h);

    std::vector<l1t::HGCalTriggerCellTruth> truthvec;

    for (const auto& wafer : *wafers_h){
        float simenergy = 0;
        std::unordered_map<int, float> simEnergyMap;
        
        for(const auto& tct : wafer.tcTruths()){
            simenergy += tct.simEnergy();
            for (const auto& sc : tct.simclusters()){
                simEnergyMap[sc.first] += sc.second * tct.simEnergy();
            }
        }
        
        int nSimClusters=0;
        float allocSimEnergyFrac=0;
        std::vector<std::pair<int, float>> simclusters;
        if (simenergy>0){
            for (const auto& it : simEnergyMap){
                simclusters.emplace_back(it.first, it.second/simenergy);
                nSimClusters++;
                allocSimEnergyFrac += it.second/simenergy;
            }
        }
        truthvec.emplace_back(
            simclusters, simenergy, 
            allocSimEnergyFrac, nSimClusters
        );
    }

    auto truthMap = std::make_unique<edm::ValueMap<l1t::HGCalTriggerCellTruth>>();
    edm::ValueMap<l1t::HGCalTriggerCellTruth>::Filler filler(*truthMap);
    filler.insert(wafers_h, truthvec.begin(), truthvec.end());
    filler.fill();
    evt.put(std::move(truthMap));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1THGCalModuleTruthProducer);
