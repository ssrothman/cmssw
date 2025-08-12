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

class L1HGCalTCTruthTableProducer : public edm::stream::EDProducer<> {
public:
    explicit L1HGCalTCTruthTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken tc_truth_token_;
    std::string name_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};

L1HGCalTCTruthTableProducer::L1HGCalTCTruthTableProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<edm::View<l1t::HGCalTriggerCell>>(conf.getParameter<edm::InputTag>("triggerCells"))),
      tc_truth_token_(consumes<edm::ValueMap<l1t::HGCalTriggerCellTruth>>(conf.getParameter<edm::InputTag>("tcTruth"))),
      name_(conf.getParameter<std::string>("name")),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
    produces<nanoaod::FlatTable>();
    produces<nanoaod::FlatTable>("truth");
}

void L1HGCalTCTruthTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void L1HGCalTCTruthTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalTriggerCell>> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);

    // Get the trigger cell truth map
    edm::Handle<edm::ValueMap<l1t::HGCalTriggerCellTruth>> tc_truth_h;
    evt.getByToken(tc_truth_token_, tc_truth_h);
    const auto& tc_truth = *tc_truth_h;

    std::vector<int> nSimClusters;
    std::vector<float> simE, matchedFrac;
    
    std::vector<int> scIdx;
    std::vector<float> scFrac;


    for(size_t i=0; i < trigger_cells_h->size(); ++i) {
        const auto& tcref = trigger_cells_h->refAt(i);
        const auto& truth = tc_truth[tcref];

        nSimClusters.push_back(truth.nSimClusters());
        simE.push_back(truth.simEnergy());
        matchedFrac.push_back(truth.allocSimEnergyFraction());

        for(const auto& sc : truth.simclusters()) {
            scIdx.push_back(sc.first);
            scFrac.push_back(sc.second);
        }
    }

    // Create the FlatTable
    auto table = std::make_unique<nanoaod::FlatTable>(simE.size(), name_.c_str(), false, true);
    table->addColumn<int>("nSimClusters", nSimClusters, "Number of associated SimClusters");
    table->addColumn<float>("simEnergy", simE, "Total energy from associated SimClusters (in GeV)");
    table->addColumn<float>("matchedSimEnergyFraction", matchedFrac, "Fraction of energy matched to SimClusters");
    evt.put(std::move(table));

    auto truthTable = std::make_unique<nanoaod::FlatTable>(scIdx.size(), (name_ + "Truth").c_str(), false, false);
    truthTable->addColumn<int>("simClusterIdx", scIdx, "Index of the associated SimCluster");
    truthTable->addColumn<float>("simClusterFraction", scFrac, "Fraction of energy from the SimCluster");
    evt.put(std::move(truthTable), "truth");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1HGCalTCTruthTableProducer);
