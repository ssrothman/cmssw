#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerSums.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class ECONTritonProducer : public TritonEDProducer<> {
  public:
    ECONTritonProducer(edm::ParameterSet const& cfg);

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    void acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) override;
    void produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) override;

    void beginRun(const edm::Run&, const edm::EventSetup&) override;
    ~ECONTritonProducer() override = default;
  private:
    edm::EDGetToken inputTCToken_;
    edm::ESHandle<HGCalTriggerGeometryBase> triggerGeometry_;
    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};

ECONTritonProducer::ECONTritonProducer(const edm::ParameterSet& cfg)
      : TritonEDProducer<>(cfg, "ECONTritonProducer"),
        inputTCToken_(consumes<l1t::HGCalTriggerCellBxCollection>(cfg.getParameter<edm::InputTag>("TriggerCells"))),
        triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
}

void ECONTritonProducer::beginRun(const edm::Run& run, const edm::EventSetup& es){
  triggerGeometry_ = es.getHandle(triggerGeomToken_);
}

void ECONTritonProducer::acquire(edm::Event const& e, edm::EventSetup const& es, Input& iInput) {
  printf("acquiring\n");
  triggerTools_.eventSetup(es);

  edm::Handle<l1t::HGCalTriggerCellBxCollection> tcs;
  e.getByToken(inputTCToken_, tcs);

  std::unordered_map<uint32_t, std::vector<l1t::HGCalTriggerCell>> tc_modules;
  for (const auto& tc : *tcs){
    uint32_t module = triggerGeometry_->getModuleFromTriggerCell(tc.detId());
    tc_modules[module].push_back(tc);
  }

  for (const auto& tc_module : tc_modules){
    const auto& detId = tc_module.second.at(0).detId();
    
    //skip scintillator modules
    if (triggerTools_.isScintillator(detId)){ 
      continue;
    }

    int thickness = triggerTools_.thicknessIndex(detId);
    HGCalTriggerTools::SubDetectorType subdet = triggerTools_.getSubDetectorType(detId);

    std::vector<uint> adc(48, 0);
    std::vector<double> AEinput(48, 0.0);

    for (const auto& tc: tc_module.second){
      HGCalTriggerDetId id(tc.detId());
      uint cellu = id.triggerCellU();
      uint cellv = id.triggerCellV();
      //int inputIndex = cellUVreman_[cellu][cellv];
      int inputIndex = 0;
      if (inputIndex < 0){
        throw cms::Exception("BadInitialization")
          << "Invalid index provided for trigger cell u=" 
          << cellu 
          << " v=" 
          << cellv 
          << " in cellUVRemap[" 
          << cellu
          << "][" << cellv << "]";
      }//end if uv lookup error

      AEinput[inputIndex] = tc.hwPt()/cosh(tc.eta());

    }//end for each trigger cell in module
  }//end for each module
  client_->setBatchSize(0);
}//end acquire

void ECONTritonProducer::produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) {
  printf("producing...\n");
}

void ECONTritonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("TriggerCells");
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(ECONTritonProducer);
