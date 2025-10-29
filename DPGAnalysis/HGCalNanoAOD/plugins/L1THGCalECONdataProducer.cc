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

class L1THGCalECONdataProducer : public edm::stream::EDProducer<> {
public:
    explicit L1THGCalECONdataProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken wafers_token_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;
    HGCalTriggerTools triggerTools_;

    unsigned bitsPerADC_;
    unsigned bitsPerNorm_;
    unsigned bitsPerCALQ_;
    unsigned bitsPerInput_;

    bool useModuleFactor_;
    bool bitShiftNormalize_;
    bool useTransverseADC_;
    bool normByMax_;

    AEinputUtil aeInputUtil_;
};

L1THGCalECONdataProducer::L1THGCalECONdataProducer(const edm::ParameterSet& conf)
    : wafers_token_(consumes<edm::View<l1t::HGCalModule>>(conf.getParameter<edm::InputTag>("wafers"))),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>()),
      bitsPerADC_(conf.getParameter<unsigned>("bitsPerADC")),
      bitsPerNorm_(conf.getParameter<unsigned>("bitsPerNorm")),
      bitsPerCALQ_(conf.getParameter<unsigned>("bitsPerCALQ")),
      bitsPerInput_(conf.getParameter<unsigned>("bitsPerInput")),
      useModuleFactor_(conf.getParameter<bool>("useModuleFactor")),
      bitShiftNormalize_(conf.getParameter<bool>("bitShiftNormalize")),
      useTransverseADC_(conf.getParameter<bool>("useTransverseADC")),
      normByMax_(conf.getParameter<bool>("normByMax")),
      aeInputUtil_(
          bitsPerADC_, bitsPerNorm_, 
          bitsPerCALQ_, bitsPerInput_,
          useModuleFactor_, bitShiftNormalize_, 
          useTransverseADC_, normByMax_) 
{

    produces<edm::ValueMap<l1t::HGCalECONdata>>();
}

void L1THGCalECONdataProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
    aeInputUtil_.setGeometry(triggerGeometry.product());
}

void L1THGCalECONdataProducer::produce(edm::Event& evt, const edm::EventSetup& es) {

    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalModule>> wafers_h;
    evt.getByToken(wafers_token_, wafers_h);


    std::vector<l1t::HGCalECONdata> econvec;

    for (const auto& wafer : *wafers_h){
        std::vector<int> ae_norm, ae_ADC, ae_CALQ;
        std::vector<float> ae_input;
        int sumCALQ;
        if(triggerTools_.isScintillator(wafer.tcId0())) {
            for(unsigned i=0; i<64; ++i){
                ae_norm.push_back(-999);
                ae_ADC.push_back(-999);
                ae_CALQ.push_back(-999);
                ae_input.push_back(-999.);
            }
            sumCALQ = -999; 
        } else {
            aeInputUtil_.run(wafer.tcs());
            for (unsigned i=0; i<64; ++i){
                ae_norm.push_back(aeInputUtil_.getNorm(i));
                ae_ADC.push_back(aeInputUtil_.getADC(i));
                ae_CALQ.push_back(aeInputUtil_.getCALQ(i));
                ae_input.push_back(aeInputUtil_.getInput(i)/aeInputUtil_.getInputNorm());
            }
            sumCALQ = aeInputUtil_.getModSum();
        }

        econvec.emplace_back(
            sumCALQ, ae_norm, ae_ADC, ae_CALQ, ae_input
        );
    }

    auto econdata = std::make_unique<edm::ValueMap<l1t::HGCalECONdata>>();
    edm::ValueMap<l1t::HGCalECONdata>::Filler filler(*econdata);
    filler.insert(wafers_h, econvec.begin(), econvec.end());
    filler.fill();
    evt.put(std::move(econdata));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1THGCalECONdataProducer);
