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
#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/OneToMany.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticleFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCEEDetId.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"

class SimClusterToTriggerCellTableProducer : public edm::stream::EDProducer<> {
public:
    explicit SimClusterToTriggerCellTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken simclusters_token_;
    std::string name_;
    std::string matchName_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};

SimClusterToTriggerCellTableProducer::SimClusterToTriggerCellTableProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<edm::View<l1t::HGCalTriggerCell>>(conf.getParameter<edm::InputTag>("triggerCells"))),
      simclusters_token_(consumes<std::vector<SimCluster>>(conf.getParameter<edm::InputTag>("simClusters"))),
      name_(conf.getParameter<std::string>("name")),
      matchName_(conf.getParameter<std::string>("matchName")),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
    produces<nanoaod::FlatTable>();
    produces<nanoaod::FlatTable>("match");
}

void SimClusterToTriggerCellTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void SimClusterToTriggerCellTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalTriggerCell>> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);

    // Get the SimClusters
    edm::Handle<std::vector<SimCluster>> simclusters_h;
    evt.getByToken(simclusters_token_, simclusters_h);

    std::unordered_map<uint32_t, uint32_t> detIdToIndexMap;
    for(unsigned i=0; i<trigger_cells_h->size(); ++i) {
        const auto& tc = trigger_cells_h->at(i);
        detIdToIndexMap[tc.detId()] = i;
    }

    std::vector<int> nmatch;

    std::vector<float> frac;
    std::vector<int> index;
    
    for(const auto& sc : *simclusters_h){
        int n=0;
        for (const auto& hitandfrac : sc.hits_and_fractions()){
            uint32_t detid = hitandfrac.first;
            float fraction = hitandfrac.second;
            auto it = detIdToIndexMap.find(detid);
            if (it != detIdToIndexMap.end()) {
                index.push_back(it->second);
                frac.push_back(fraction);
                ++n;
            }
        }
        nmatch.push_back(n);
    }

    // Create the FlatTable
    auto table = std::make_unique<nanoaod::FlatTable>(nmatch.size(), name_.c_str(), false, true);
    table->addColumn<int>("nMatchedHits", nmatch, "Number of matched hits in the trigger cells");
    evt.put(std::move(table));

    auto matchTable = std::make_unique<nanoaod::FlatTable>(frac.size(), matchName_.c_str(), false, false);
    matchTable->addColumn<int>("index", index, "Index of the trigger cell in the event");
    matchTable->addColumn<float>("fraction", frac, "Fraction of the SimCluster energy in the trigger cell");
    evt.put(std::move(matchTable), "match");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SimClusterToTriggerCellTableProducer);

