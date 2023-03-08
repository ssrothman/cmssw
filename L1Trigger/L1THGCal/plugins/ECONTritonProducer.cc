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
};

ECONTritonProducer::ECONTritonProducer(const edm::ParameterSet& cfg)
      : TritonEDProducer<>(cfg),
        inputTCToken_(consumes<l1t::HGCalTriggerCellBxCollection>(cfg.getParameter<edm::InputTag>("TriggerCells"))),
        triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
  produces<AEMap>();
  produces<ECONMap>();
}

void ECONTritonProducer::beginRun(const edm::Run& run, const edm::EventSetup& es){
  triggerGeometry_ = es.getHandle(triggerGeomToken_);
  triggerTools_.eventSetup(es, triggerGeomToken_);
}

void ECONTritonProducer::acquire(edm::Event const& e, edm::EventSetup const& es, Input& iInput) {
  printf("acquiring\n");
  tc_modules_.clear();

  edm::Handle<l1t::HGCalTriggerCellBxCollection> tcs;
  e.getByToken(inputTCToken_, tcs);

  for (const auto& tc : *tcs){
    uint32_t module = triggerGeometry_->getModuleFromTriggerCell(tc.detId());
    tc_modules_[module].push_back(tc);
  }
  printf("tc modules map filled\n");

  nModule_ = 0;
  for (const auto& tc_module: tc_modules_){
    if (!triggerTools_.isScintillator(tc_module.second.at(0).detId())){
      ++nModule_;
    }
  }

  client_->setBatchSize(nModule_);
  printf("batch size was set\n");

  auto& inputCALQ = iInput.begin()->second;;
  auto dataCALQ = std::make_shared<TritonInput<float>>();

  unsigned i=0;
  bool printed = false;
  for (const auto& tc_module : tc_modules_){
    //skip scintillator modules
    const auto& detId = tc_module.second.at(0).detId();
    if (triggerTools_.isScintillator(detId)){ 
      continue;
    }

    int thickness = triggerTools_.thicknessIndex(detId);
    HGCalTriggerTools::SubDetectorType subdet = triggerTools_.getSubDetectorType(detId);

    std::vector<float> AEinput_module(nTriggerCells, 0.0);

    for (const auto& tc: tc_module.second){
      HGCalTriggerDetId id(tc.detId());
      unsigned cellu = id.triggerCellU();
      unsigned cellv = id.triggerCellV();
      int inputIndex = cellUVremap[cellu][cellv];
      if(!printed){
        printf("theres a nonzero trigger cell (%u, %u)[%d] = %d\n", cellu, cellv, inputIndex, tc.hwPt());
      }
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

      AEinput_module[inputIndex] = tc.hwPt()/cosh(tc.eta());
    }//end for each trigger cell in module

    if(!printed){
      printf("input for module %u is:\n\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, ...\n",
          tc_module.first,
          AEinput_module[0],
          AEinput_module[1],
          AEinput_module[2],
          AEinput_module[3],
          AEinput_module[4]);
      printed=true;
    }
    dataCALQ->push_back(AEinput_module);
    ++nModule_;
  }//end for each module
  printf("filled inputs\n");

  printf("input dims = %lu, %lu\n", dataCALQ->size(), dataCALQ->at(0).size());
  printf("I stg the inputs are:\n\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[0],
          dataCALQ->at(0)[1],
          dataCALQ->at(0)[2],
          dataCALQ->at(0)[3],
          dataCALQ->at(0)[4],
          dataCALQ->at(0)[5]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[6],
          dataCALQ->at(0)[7],
          dataCALQ->at(0)[8],
          dataCALQ->at(0)[9],
          dataCALQ->at(0)[10],
          dataCALQ->at(0)[11]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[12],
          dataCALQ->at(0)[13],
          dataCALQ->at(0)[14],
          dataCALQ->at(0)[15],
          dataCALQ->at(0)[16],
          dataCALQ->at(0)[17]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[18],
          dataCALQ->at(0)[19],
          dataCALQ->at(0)[20],
          dataCALQ->at(0)[21],
          dataCALQ->at(0)[22],
          dataCALQ->at(0)[23]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[24],
          dataCALQ->at(0)[25],
          dataCALQ->at(0)[26],
          dataCALQ->at(0)[27],
          dataCALQ->at(0)[28],
          dataCALQ->at(0)[29]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[30],
          dataCALQ->at(0)[31],
          dataCALQ->at(0)[32],
          dataCALQ->at(0)[33],
          dataCALQ->at(0)[34],
          dataCALQ->at(0)[35]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[36],
          dataCALQ->at(0)[37],
          dataCALQ->at(0)[38],
          dataCALQ->at(0)[39],
          dataCALQ->at(0)[40],
          dataCALQ->at(0)[41]);
  printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f %0.1f\n",
          dataCALQ->at(0)[42],
          dataCALQ->at(0)[43],
          dataCALQ->at(0)[44],
          dataCALQ->at(0)[45],
          dataCALQ->at(0)[46],
          dataCALQ->at(0)[47]);
  inputCALQ.toServer(dataCALQ);
  printf("end acquire()\n");

}//end acquire

void ECONTritonProducer::produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) {
  for (const auto& iter: iOutput){
    std::cout << iter.first << std::endl;
  }
  printf("producing...\n");
  const auto& ECONout = iOutput.at("ECON__0").fromServer<float>();
  printf("got ECONout\n");
  const auto& CALQout = iOutput.at("rCALQ__1").fromServer<float>();
  printf("got CALQout\n");
  printf("output dims = %lu, %lu\n", CALQout.size(), CALQout.at(0).size());

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

    std::array<float, nTriggerCells> AE_wafer;
    for (int iTC=0; iTC<nTriggerCells; ++iTC){
      int cellU = ae_outputCellU[iTC];
      int cellV = ae_outputCellV[iTC];
      HGCalTriggerDetId id(subdet, zside, type, layer, waferU, waferV, cellU, cellV);
      //if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
      //  continue;
      //}

      float adc = CALQout[iModule][iTC];
      AE_wafer[iTC] = adc;
    }
    if(!printed){
      printed=true;
      printf("output for module %u is:\n\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          tc_module.first,
          AE_wafer[0],
          AE_wafer[1],
          AE_wafer[2],
          AE_wafer[3],
          AE_wafer[4],
          AE_wafer[5]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[6],
          AE_wafer[7],
          AE_wafer[8],
          AE_wafer[9],
          AE_wafer[10],
          AE_wafer[11]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[12],
          AE_wafer[13],
          AE_wafer[14],
          AE_wafer[15],
          AE_wafer[16],
          AE_wafer[17]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[18],
          AE_wafer[19],
          AE_wafer[20],
          AE_wafer[21],
          AE_wafer[22],
          AE_wafer[23]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[24],
          AE_wafer[25],
          AE_wafer[26],
          AE_wafer[27],
          AE_wafer[28],
          AE_wafer[29]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[30],
          AE_wafer[31],
          AE_wafer[32],
          AE_wafer[33],
          AE_wafer[34],
          AE_wafer[35]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[36],
          AE_wafer[37],
          AE_wafer[38],
          AE_wafer[39],
          AE_wafer[40],
          AE_wafer[41]);
      printf("\t%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f\n",
          AE_wafer[42],
          AE_wafer[43],
          AE_wafer[44],
          AE_wafer[45],
          AE_wafer[46],
          AE_wafer[47]);
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
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(ECONTritonProducer);
