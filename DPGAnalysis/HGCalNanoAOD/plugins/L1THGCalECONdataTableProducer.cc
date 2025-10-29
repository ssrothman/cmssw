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

class L1HGCalECONdataTableProducer : public edm::stream::EDProducer<> {
public:
    explicit L1HGCalECONdataTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken wafer_token_;
    edm::EDGetToken ECONdata_token_;
    std::string name_;

};

L1HGCalECONdataTableProducer::L1HGCalECONdataTableProducer(const edm::ParameterSet& conf)
    : wafer_token_(consumes<edm::View<l1t::HGCalModule>>(conf.getParameter<edm::InputTag>("wafers"))),
      ECONdata_token_(consumes<edm::ValueMap<l1t::HGCalECONdata>>(conf.getParameter<edm::InputTag>("ECONdata"))),
      name_(conf.getParameter<std::string>("name"))
{
    produces<nanoaod::FlatTable>();
    produces<nanoaod::FlatTable>("ECONdata");
}

void L1HGCalECONdataTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
}

void L1HGCalECONdataTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    edm::Handle<edm::View<l1t::HGCalModule>> wafers_h;
    evt.getByToken(wafer_token_, wafers_h);

    edm::Handle<edm::ValueMap<l1t::HGCalECONdata>> ECONdata_h;
    evt.getByToken(ECONdata_token_, ECONdata_h);

    //common
    std::vector<uint32_t> sumCALQ;
    std::vector<uint32_t> ae_norm, ae_ADC, ae_CALQ;
    std::vector<float> ae_input;

    for(size_t i=0; i<wafers_h->size(); ++i) {
        const auto& econdata = (*ECONdata_h)[wafers_h->refAt(i)];
        sumCALQ.push_back(econdata.getSumCALQ());
        ae_norm.insert(ae_norm.end(), econdata.getAENorm().begin(), econdata.getAENorm().end());
        ae_ADC.insert(ae_ADC.end(), econdata.getAEADC().begin(), econdata.getAEADC().end());
        ae_CALQ.insert(ae_CALQ.end(), econdata.getAECALQ().begin(), econdata.getAECALQ().end());
        ae_input.insert(ae_input.end(), econdata.getAEInput().begin(), econdata.getAEInput().end());
    }

    // Create the FlatTable
    auto table = std::make_unique<nanoaod::FlatTable>(sumCALQ.size(), name_.c_str(), false, true);
    table->addColumn<int>("sumCALQ", sumCALQ, "Sum CALQ");
    evt.put(std::move(table));

    auto econ_table = std::make_unique<nanoaod::FlatTable>(ae_norm.size(), (name_ + "ECONdata").c_str(), false, false);
    econ_table->addColumn<uint32_t>("ae_norm", ae_norm, "AE Norm");
    econ_table->addColumn<uint32_t>("ae_ADC", ae_ADC, "AE ADC");
    econ_table->addColumn<uint32_t>("ae_CALQ", ae_CALQ, "AE CALQ");
    econ_table->addColumn<uint32_t>("ae_input", ae_input, "AE Input");
    evt.put(std::move(econ_table), "ECONdata");

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1HGCalECONdataTableProducer);
