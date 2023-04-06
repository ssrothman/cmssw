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

#include "L1Trigger/L1THGCal/interface/AEutil.h"
#include "L1Trigger/L1THGCal/interface/TCSelector.h"

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

    size_t nModule_;
    std::unordered_map<uint32_t, std::vector<l1t::HGCalTriggerCell>> tc_modules_;
    std::unordered_map<uint32_t, double> modSums_;

    //what variable to use as AE input
    InputType inType_;
    NormType normType_;

    TCSelector selector_;
};

ECONTritonProducer::ECONTritonProducer(const edm::ParameterSet& cfg)
      : TritonEDProducer<>(cfg),
        inputTCToken_(consumes<l1t::HGCalTriggerCellBxCollection>(cfg.getParameter<edm::InputTag>("TriggerCells"))),
        triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>()),
        selector_(cfg.getParameter<std::string>("cut"))
{
  inType_ = inputTypeStrToEnum(cfg.getParameter<std::string>("inputType"));
  normType_ = normTypeStrToEnum(cfg.getParameter<std::string>("normType"));
  produces<AEMap>();
  produces<ECONMap>();
}

void ECONTritonProducer::beginRun(const edm::Run& run, const edm::EventSetup& es){
  triggerGeometry_ = es.getHandle(triggerGeomToken_);
  triggerTools_.eventSetup(es, triggerGeomToken_);
}

void ECONTritonProducer::acquire(edm::Event const& e, edm::EventSetup const& es, Input& iInput) {
  tc_modules_.clear();
  modSums_.clear();

  edm::Handle<l1t::HGCalTriggerCellBxCollection> tcs;
  e.getByToken(inputTCToken_, tcs);

  for (const auto& tc : *tcs){
    uint32_t module = triggerGeometry_->getModuleFromTriggerCell(tc.detId());
    tc_modules_[module].push_back(tc);
  }

  nModule_ = 0;
  for (const auto& tc_module: tc_modules_){
    if (!triggerTools_.isScintillator(tc_module.second.at(0).detId())
        && selector_(tc_module.second.at(0).detId())){
      ++nModule_;
    }
  }

  std::cout << "Working on " << nModule_ << " modules" << std::endl;
  client_->setBatchSize(nModule_);

  auto& inputCALQ = iInput.begin()->second;;
  auto dataCALQ = std::make_shared<TritonInput<float>>();

  for (const auto& tc_module : tc_modules_){
    //skip scintillator modules
    const auto& detId = tc_module.second.at(0).detId();
    if (triggerTools_.isScintillator(detId) || !selector_(detId)){ 
      continue;
    }

    std::vector<float> AEinput_module(nTriggerCells, 0.0);

    for (const auto& tc: tc_module.second){
      HGCalTriggerDetId id(tc.detId());
      unsigned cellu = id.triggerCellU();
      unsigned cellv = id.triggerCellV();
      int inputIndex = cellUVremap[cellu][cellv];
      
      //int inputIndex = 0;
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

      switch (inType_){
        case ADC:
          AEinput_module[inputIndex] = tc.hwPt();
          break;
        case ADCT:
          AEinput_module[inputIndex] = tc.hwPt()/cosh(tc.eta());
          break;
        case MIPT:
          AEinput_module[inputIndex] = tc.mipPt();
          break;
        case MIP:
          AEinput_module[inputIndex] = tc.mipPt()*cosh(tc.eta());
          break;
        case E:
          AEinput_module[inputIndex] = tc.energy();
          break;
        case ET:
          AEinput_module[inputIndex] = tc.energy()/cosh(tc.eta());
          break;
        default:
          throw cms::Exception("ECONTritonProducer") << "Invalid InputType";
          break;
      }
      //add AE input to modSum
      modSums_[tc_module.first] += AEinput_module[inputIndex];
    }//end for each trigger cell in module

    if(normType_ != NormType::None){
      double normalization = 1;
      if(normType_ == NormType::Floating){
        normalization = modSums_[tc_module.first];
      } else {
        int msb = int(log2(modSums_[tc_module.first]));
        normalization = pow(2, msb);
      }
      for(int iTC=0; iTC<nTriggerCells; ++iTC){
        AEinput_module[iTC]/=normalization;
      }
    }

    dataCALQ->push_back(AEinput_module);
    ++nModule_;
  }//end for each module
  inputCALQ.toServer(dataCALQ);

}//end acquire

void ECONTritonProducer::produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) {
  if(nModule_<=0){//put empty maps and return
    auto ADCmap = std::make_unique<AEMap>();
    auto latentMap = std::make_unique<ECONMap>();
    iEvent.put(std::move(ADCmap));
    iEvent.put(std::move(latentMap));
    return;
  }
  const auto& ECONout = iOutput.at("ECON__0").fromServer<float>();
  const auto& CALQout = iOutput.at("rCALQ__1").fromServer<float>();

  auto ADCmap = std::make_unique<AEMap>();
  auto latentMap = std::make_unique<ECONMap>();

  size_t iModule=0;
  bool printed=false;
  for(const auto& tc_module : tc_modules_){
    //skip scintillator modules
    if (triggerTools_.isScintillator(tc_module.second.at(0).detId())){ 
      continue;
    }

    HGCalTriggerDetId firstid(tc_module.second.at(0).detId());
    if (!selector_(firstid)){
      continue;
    }

    int subdet = firstid.subdet();
    int zside = firstid.zside();
    int type = firstid.type();
    int layer = firstid.layer();
    int waferU = firstid.waferU();
    int waferV = firstid.waferV();
    
    std::array<float, latentSize> latent_wafer;
    for (int iDim=0; iDim<latentSize; ++iDim){
      latent_wafer[iDim] = ECONout[iModule][iDim];
    }
    (*latentMap)[tc_module.first] = latent_wafer;

    double AEmodSum=0;
    for (int iTC=0; iTC<nTriggerCells; ++iTC){//loop to compute AEmodSum
      int cellU = ae_outputCellU[iTC];
      int cellV = ae_outputCellV[iTC];
      HGCalTriggerDetId id(subdet, zside, type, layer, waferU, waferV, cellU, cellV);
      if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
        continue;
      }
      AEmodSum += std::max<float>(CALQout[iModule][iTC], 0.f);
    }//end loop to compute AEmodSum

    double normalization = 1;
    if(normType_ != NormType::None){
      if(normType_ == NormType::Floating){
        normalization = modSums_[tc_module.first];
      } else{
        int msb = int(log2(modSums_[tc_module.first]));
        normalization = pow(2, msb);
      }
    }
    
    double renormalization = 1;
    if(normType_ != NormType::None){
      if(normType_ == NormType::Floating){
        renormalization = 1/AEmodSum;
      } else {
        renormalization = modSums_[tc_module.first]/(AEmodSum*normalization);
      }
    }

    std::array<float, nTriggerCells> AE_wafer;
    for (int iTC=0; iTC<nTriggerCells; ++iTC){//loop to fill computed AE values
      int cellU = ae_outputCellU[iTC];
      int cellV = ae_outputCellV[iTC];
      HGCalTriggerDetId id(subdet, zside, type, layer, waferU, waferV, cellU, cellV);
      if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
        continue;
      }

      float ans = std::max<float>(CALQout[iModule][iTC], 0.f);
      AE_wafer[iTC] = ans * normalization * renormalization;

      if(!printed){
        printf("WAFER %d:\n", tc_module.first);
        printf("\tTYPE = %d\n", id.type());
        printf("\toriginal wafer sum = %0.3lf\n",(modSums_[tc_module.first]));
        printf("\toriginal normalization = %0.3lf\n",normalization);
        printf("\toutput sum = %0.3lf\n", AEmodSum);
        printf("\trenormalization = %0.3lf\n", renormalization);
        printf("\toutput value = %0.3f\n", ans);
        printf("\toutput sum * normalization * renormalization = %0.3f\n", AEmodSum*normalization * renormalization);
        printf("\n");
        printed=true;
      }
    }

    (*ADCmap)[tc_module.first] = AE_wafer;
    ++iModule;
  }


  iEvent.put(std::move(ADCmap));
  iEvent.put(std::move(latentMap));
}

void ECONTritonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("TriggerCells");
  desc.add<std::string>("inputType");
  desc.add<std::string>("normType");
  desc.add<std::string>("cut");
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(ECONTritonProducer);
