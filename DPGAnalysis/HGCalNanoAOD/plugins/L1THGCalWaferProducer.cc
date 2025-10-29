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
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"

#include "L1Trigger/L1THGCal/interface/concentrator/AEinputUtil.h"

class L1THGCalModuleProducer : public edm::stream::EDProducer<> {
public:
    explicit L1THGCalModuleProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken tc_truth_token_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;
    HGCalTriggerTools triggerTools_;
};

L1THGCalModuleProducer::L1THGCalModuleProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<edm::View<l1t::HGCalTriggerCell>>(conf.getParameter<edm::InputTag>("triggerCells"))),
      tc_truth_token_(consumes<edm::ValueMap<l1t::HGCalTriggerCellTruth>>(conf.getParameter<edm::InputTag>("tcTruth"))),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{

    produces<std::vector<l1t::HGCalModule>>();
}

void L1THGCalModuleProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void L1THGCalModuleProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalTriggerCell>> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);

    // Get the trigger cell truth map
    edm::Handle<edm::ValueMap<l1t::HGCalTriggerCellTruth>> tc_truth_h;
    evt.getByToken(tc_truth_token_, tc_truth_h);
    const auto& tc_truth = *tc_truth_h;

    std::unordered_map<uint32_t, std::vector<size_t>> wafers;
    for(size_t itc=0; itc < trigger_cells_h->size(); ++itc) {
        const auto& tc = trigger_cells_h->at(itc);
        uint32_t wafer_id = triggerTools_.getTriggerGeometry()->getModuleFromTriggerCell(tc.detId());
        wafers[wafer_id].push_back(itc);
    }

    auto wafers_out = std::make_unique<std::vector<l1t::HGCalModule>>();

    for (const auto& wafer : wafers){
        std::vector<l1t::HGCalTriggerCell> tcs;
        std::vector<l1t::HGCalTriggerCellTruth> tc_truths;

        float energy=0;
        float mipPt=0;
        
        for(const auto& itc : wafer.second) {
            energy += trigger_cells_h->at(itc).energy();
            mipPt += trigger_cells_h->at(itc).mipPt();

            tcs.push_back(trigger_cells_h->at(itc));

            const auto& truth = tc_truth[trigger_cells_h->refAt(itc)];
            tc_truths.push_back(truth);
        }
        uint32_t id0 = tcs[0].detId();
        wafers_out->emplace_back(
            wafer.first, id0, 
            energy, mipPt,
            tcs, tc_truths
        );
    }

    evt.put(std::move(wafers_out));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1THGCalModuleProducer);
