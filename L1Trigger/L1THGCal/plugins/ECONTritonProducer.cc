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
//#include "DataFormats/L1THGCal/interface/AEMap.h"

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using AEMap=std::unordered_map<unsigned int, float>;

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

    size_t nModule_;
    std::unordered_map<uint32_t, std::vector<l1t::HGCalTriggerCell>> tc_modules_;
};

ECONTritonProducer::ECONTritonProducer(const edm::ParameterSet& cfg)
      : TritonEDProducer<>(cfg, "ECONTritonProducer"),
        inputTCToken_(consumes<l1t::HGCalTriggerCellBxCollection>(cfg.getParameter<edm::InputTag>("TriggerCells"))),
        triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
  produces<AEMap>();
}

void ECONTritonProducer::beginRun(const edm::Run& run, const edm::EventSetup& es){
  triggerGeometry_ = es.getHandle(triggerGeomToken_);
}

void ECONTritonProducer::acquire(edm::Event const& e, edm::EventSetup const& es, Input& iInput) {
  printf("acquiring\n");

  client_->setBatchSize(12845);
  printf("batch size was set\n");

  tc_modules_.clear();

  triggerTools_.eventSetup(es);

  edm::Handle<l1t::HGCalTriggerCellBxCollection> tcs;
  e.getByToken(inputTCToken_, tcs);

  for (const auto& tc : *tcs){
    uint32_t module = triggerGeometry_->getModuleFromTriggerCell(tc.detId());
    tc_modules_[module].push_back(tc);
  }
  printf("tc modules map filled\n");

  nModule_ = 0;

  auto& inputCALQ = iInput.begin()->second;;
  auto dataCALQ = std::make_shared<TritonInput<float>>();

  for (const auto& tc_module : tc_modules_){
    //skip scintillator modules
    const auto& detId = tc_module.second.at(0).detId();
    if (triggerTools_.isScintillator(detId)){ 
      continue;
    }

    int thickness = triggerTools_.thicknessIndex(detId);
    HGCalTriggerTools::SubDetectorType subdet = triggerTools_.getSubDetectorType(detId);

    std::vector<float> AEinput_module(48.0, 0.0);

    for (const auto& tc: tc_module.second){
      HGCalTriggerDetId id(tc.detId());
      uint cellu = id.triggerCellU();
      uint cellv = id.triggerCellV();
      //int inputIndex = cellUVremap_[cellu][cellv];
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

      AEinput_module[inputIndex] = tc.hwPt()/cosh(tc.eta());
    }//end for each trigger cell in module

    dataCALQ->push_back(AEinput_module);
    ++nModule_;
  }//end for each module
  printf("filled inputs\n");

  inputCALQ.toServer(dataCALQ);
  printf("end acquire()\n");

}//end acquire

void ECONTritonProducer::produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) {
  printf("producing...\n");
  const auto& ECONout = iOutput.at("ECON").fromServer<float>();
  const auto& CALQout = iOutput.at("rCALQ").fromServer<float>();

  auto ADCmap = std::make_unique<AEMap>();

  size_t iModule=0;
  for(const auto& tc_module : tc_modules_){
    //skip scintillator modules
    if (triggerTools_.isScintillator(tc_module.second.at(0).detId())){ 
      continue;
    }

    HGCalTriggerDetId firstid(tc_module.second.at(0).detId());
    int subdet = firstid.subdet();
    int zside = firstid.zside();
    int type = firstid.type();
    int layer = firstid.layer();
    int waferU = firstid.waferU();
    int waferV = firstid.waferV();
    
    for (int iTC=0; iTC<48; ++iTC){
      //int cellU = ae_outputCellU_[i];
      //int cellV = ae_outputCellV_[i];
      int cellU = 0;
      int cellV = 0;
      HGCalTriggerDetId id(subdet, zside, type, layer, waferU, waferV, cellU, cellV);
      if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
        continue;
      }
      //GlobalPoint point = triggerTools_.getTCPosition(id);

      //double adc = CALQout.at(iTC + 48*iModule);
      float adc = CALQout[iModule][iTC];
      (*ADCmap)[id()] = adc;
    }
    ++iModule;
  }

  iEvent.put(std::move(ADCmap));
}

void ECONTritonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("TriggerCells");
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(ECONTritonProducer);
