#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorAEFromTritonImpl.h"

HGCalConcentratorAEFromTritonImpl::HGCalConcentratorAEFromTritonImpl(const edm::ParameterSet& conf) 
    : bitsPerADC_(conf.getParameter<unsigned>("bitsPerADC")),
      bitsPerNorm_(conf.getParameter<unsigned>("bitsPerNorm")),
      bitsPerCALQ_(conf.getParameter<unsigned>("bitsPerCALQ")),
      bitsPerInput_(conf.getParameter<unsigned>("nBitsPerInput")),
      useModuleFactor_(conf.getParameter<bool>("useModuleFactor")),
      bitShiftNormalization_(conf.getParameter<bool>("bitShiftNormalization")),
      useTransverseADC_(conf.getParameter<bool>("useTransverseADC")),
      normByMax_(conf.getParameter<bool>("normByMax")),
      aeInputUtil_(bitsPerADC_, bitsPerNorm_, bitsPerCALQ_, bitsPerInput_, useModuleFactor_, bitShiftNormalization_, useTransverseADC_, normByMax_){}

void HGCalConcentratorAEFromTritonImpl::setGeometry(const HGCalTriggerGeometryBase *const geom){
  geometry_ = geom;
  triggerTools_.setGeometry(geometry_);
  aeInputUtil_.setGeometry(geometry_);
}

void HGCalConcentratorAEFromTritonImpl::select(unsigned moduleID, 
                                               const l1t::HGCalTriggerCell& tc0,
                                               std::vector<l1t::HGCalTriggerCell>& trigCellVecOutput,
                                               std::vector<l1t::HGCalConcentratorData>& ae_encodedLayer_Output) {

  HGCalTriggerDetId id(tc0.detId());

  int subdet = id.subdet();
  int zp = id.zside();
  int type = id.type();
  int layer = id.layer();
  int waferU = id.waferU();
  int waferV = id.waferV();

  //use first TC to find mipPt conversions to Et and ADC
  float mipPtToEt_conv = tc0.et() / tc0.mipPt();
  float mipToADC_conv = tc0.hwPt() / (tc0.mipPt() * cosh(tc0.eta()));

  const std::array<float, nTriggerCells> AE_wafer = AEout()->at(moduleID);

  //for each trigger cell in the module
  for(unsigned iTC=0; iTC<nTriggerCells; ++iTC){
    int cellU = aeInputUtil_.getU(iTC);
    int cellV = aeInputUtil_.getV(iTC);

    HGCalTriggerDetId id(subdet, zp, type, layer, waferU, waferV, cellU, cellV);

    //if invalid tc, skip
    if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
      continue;
    }

    const float CALQ = AE_wafer[iTC];

    if (CALQ<=0)
      continue;
  
    GlobalPoint point = geometry_->getTriggerCellPosition(id());

    double adc = aeInputUtil_.CALQtoADC(CALQ, cellU, cellV);
    double mipPt = adc / mipToADC_conv / cosh(point.eta());
    double et = mipPt * mipPtToEt_conv;


    l1t::HGCalTriggerCell triggerCell(reco::LeafCandidate::LorentzVector(), adc, 0, 0, 0, id);
    triggerCell.setCompressedCharge(999);
    triggerCell.setUncompressedCharge(999);
    triggerCell.setMipPt(mipPt);


    //build 4-vector
    math::PtEtaPhiMLorentzVector p4(et, point.eta(), point.phi(), 0.);

    //build tc object
    triggerCell.setP4(p4);
    triggerCell.setPosition(point);

  }//end loop over trigger cells
  const std::array<float, latentSize> ECON_wafer = ECONout()->at(moduleID);

  id = HGCalTriggerDetId(subdet, zp, type, layer, waferU, waferV, 0, 0);
  for (unsigned iDim=0; iDim<latentSize; ++iDim){
    ae_encodedLayer_Output.emplace_back(ECON_wafer[iDim], iDim, id); 
  }
}//end select
