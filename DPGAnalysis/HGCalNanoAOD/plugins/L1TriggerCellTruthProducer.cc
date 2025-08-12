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
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"

class L1TriggerCellTruthProducer : public edm::stream::EDProducer<> {
public:
    explicit L1TriggerCellTruthProducer(const edm::ParameterSet&);

    ~L1TriggerCellTruthProducer() override;

    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;

private:
    double calibrate(double energy, unsigned thickness, unsigned layer);

    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken simhits_ee_token_;
    edm::EDGetToken simhits_fh_token_;
    edm::EDGetToken simhits_bh_token_;
    edm::EDGetToken simclusters_token_;

    double keV2fC_;
    std::vector<double> fcPerMip_;
    std::vector<double> layerWeights_;
    std::vector<double> thicknessCorrections_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};

L1TriggerCellTruthProducer::L1TriggerCellTruthProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<l1t::HGCalTriggerCellBxCollection>(conf.getParameter<edm::InputTag>("triggerCells"))),
      simhits_ee_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsEE"))),
      simhits_fh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsFH"))),
      simhits_bh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsBH"))),
      simclusters_token_(consumes<std::vector<SimCluster>>(conf.getParameter<edm::InputTag>("simClusters"))),
      keV2fC_(conf.getParameter<double>("keV2fC")),
      fcPerMip_(conf.getParameter<std::vector<double>>("fcPerMip")),
      layerWeights_(conf.getParameter<std::vector<double>>("layerWeights")),
      thicknessCorrections_(conf.getParameter<std::vector<double>>("thicknessCorrections")),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>()) {
    produces<edm::ValueMap<l1t::HGCalTriggerCellTruth>>();
}

L1TriggerCellTruthProducer::~L1TriggerCellTruthProducer() {}

void L1TriggerCellTruthProducer::beginRun(const edm::Run& run, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

double L1TriggerCellTruthProducer::calibrate(double energy, unsigned thickness, unsigned layer) {
    double fcPerMip = fcPerMip_[thickness];
    double thicknessCorrection = thicknessCorrections_[thickness];
    double layerWeight = layerWeights_[layer];
    double TeV2GeV = 1000.0;
  return energy * keV2fC_ / fcPerMip * layerWeight * TeV2GeV / thicknessCorrection;
}

void L1TriggerCellTruthProducer::produce(edm::Event& event, const edm::EventSetup& setup) {
    std::vector<l1t::HGCalTriggerCellTruth> triggerCellsTruth;

    edm::Handle<l1t::HGCalTriggerCellBxCollection> triggerCells;
    event.getByToken(trigger_cells_token_, triggerCells);

    edm::Handle<edm::PCaloHitContainer> simHitsEE;
    event.getByToken(simhits_ee_token_, simHitsEE);
    edm::Handle<edm::PCaloHitContainer> simHitsFH;
    event.getByToken(simhits_fh_token_, simHitsFH);
    edm::Handle<edm::PCaloHitContainer> simHitsBH;
    event.getByToken(simhits_bh_token_, simHitsBH);

    edm::Handle<std::vector<SimCluster>> simClusters;
    event.getByToken(simclusters_token_, simClusters);

    std::unordered_map<uint32_t, std::vector<PCaloHit>> simhits_map;

    std::unordered_map<uint32_t, std::vector<std::pair<int, float>>> simclusters_map;

    // Fill the simhits map
    for (const auto& hit : *simHitsEE) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
    }
    for (const auto& hit : *simHitsFH) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
    }
    for (const auto& hit : *simHitsBH) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
    }

    // Fill the simclusters map
    for (uint32_t iSimCluster = 0; iSimCluster < simClusters->size(); ++iSimCluster) {
        const auto& simcluster = (*simClusters)[iSimCluster];
        for (const auto& hit : simcluster.hits_and_fractions()){
            uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.first);
            if (tcid != 0) { // Only consider valid trigger cells
                simclusters_map[hit.first].emplace_back(iSimCluster, hit.second);
            }
        }
    }

    for (const auto& tc : *triggerCells) {
        uint32_t idint = tc.detId();

        DetId id(idint);
    
        const auto& simhitvec = simhits_map.find(idint); 
        float simE = 0.0;
        float matchedSimE = 0.0;
        std::vector<std::pair<int, float>> simclusters;

        if (simhitvec != simhits_map.end()){
            std::unordered_map<uint32_t, float> simcluster_lookup;
            for (const auto& simhit : simhitvec->second) {
                unsigned thickness = triggerTools_.thicknessIndex(simhit.id());
                unsigned layer = triggerTools_.layerWithOffset(simhit.id());
                float E = calibrate(simhit.energy(), thickness, layer);

                simE += E;

                const auto& simclusters = simclusters_map.find(simhit.id());
                if (simclusters != simclusters_map.end()) {
                    for (const auto& simcluster : simclusters->second) {
                        matchedSimE += simcluster.second * E;
                        simcluster_lookup[simcluster.first] += simcluster.second * E;
                    }
                }
            } 

            for (const auto& simcluster : simcluster_lookup) {
                simclusters.emplace_back(
                    simcluster.first, 
                    simcluster.second / simE
                );
            }
        }

        float simEfrac = 1.0;
        if (simE > 0.0) {
            simEfrac = matchedSimE / simE;
        }

        triggerCellsTruth.emplace_back(
            simclusters,
            simE, 
            simEfrac,
            simclusters.size()
        );
    }

    auto triggerCellsTruthValueMap = std::make_unique<edm::ValueMap<l1t::HGCalTriggerCellTruth>>();
    edm::ValueMap<l1t::HGCalTriggerCellTruth>::Filler filler(*triggerCellsTruthValueMap);
    filler.insert(triggerCells, triggerCellsTruth.begin(), triggerCellsTruth.end());
    filler.fill();

    event.put(std::move(triggerCellsTruthValueMap));
}

DEFINE_FWK_MODULE(L1TriggerCellTruthProducer);
