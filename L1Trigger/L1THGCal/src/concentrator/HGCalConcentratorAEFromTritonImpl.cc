#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorAEFromTritonImpl.h"

HGCalConcentratorAEFromTritonImpl::HGCalConcentratorAEFromTritonImpl(const edm::ParameterSet& conf) {
    calibrationEE_ =
            std::make_unique<HGCalTriggerCellCalibration>(conf.getParameter<edm::ParameterSet>("calibrationCfg_ee"));
    calibrationHEsi_ =
            std::make_unique<HGCalTriggerCellCalibration>(conf.getParameter<edm::ParameterSet>("calibrationCfg_hesi"));
    calibrationHEsc_ =
            std::make_unique<HGCalTriggerCellCalibration>(conf.getParameter<edm::ParameterSet>("calibrationCfg_hesc"));
    calibrationNose_ =
            std::make_unique<HGCalTriggerCellCalibration>(conf.getParameter<edm::ParameterSet>("calibrationCfg_nose"));
}

void HGCalConcentratorAEFromTritonImpl::setGeometry(const HGCalTriggerGeometryBase *const geom){
  geometry_ = geom;
  calibrationEE_->setGeometry(geometry_);
  calibrationHEsi_->setGeometry(geometry_);
  calibrationHEsc_->setGeometry(geometry_);
  calibrationNose_->setGeometry(geometry_);
  triggerTools_.setGeometry(geometry_);
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

  bool isSilicon = triggerTools_.isSilicon(id);
  bool isEM = triggerTools_.isEm(id);
  bool isNose = triggerTools_.isNose(id);

  const std::array<float, nTriggerCells> AE_wafer = AEout()->at(moduleID);

  //for each trigger cell in the module
  for(unsigned iTC=0; iTC<nTriggerCells; ++iTC){
    int cellU = ae_outputCellU[iTC];
    int cellV = ae_outputCellV[iTC];

    HGCalTriggerDetId id(subdet, zp, type, layer, waferU, waferV, cellU, cellV);

    //if invalid tc, skip
    if(!triggerTools_.getTriggerGeometry()->validTriggerCell(id)){
      continue;
    }

    //TODO: actually get AEpt
    const float AEpt = AE_wafer[iTC];

    if (AEpt<=0)
      continue;
  
    GlobalPoint point = geometry_->getTriggerCellPosition(id());

    //AE is predicting transverse charge. Proper charge is AEpt*cosh(eta)
    double AEQ = AEpt*cosh(point.eta());
     
    //build 4-vector
    math::PtEtaPhiMLorentzVector p4(AEpt, point.eta(), point.phi(), 0.);

    //build tc object
    l1t::HGCalTriggerCell triggerCell(reco::LeafCandidate::LorentzVector(), AEQ, 0, 0, 0, id());
    //I hope these variables are not too important
    triggerCell.setCompressedCharge(999);
    triggerCell.setUncompressedCharge(999);
    triggerCell.setP4(p4);
    triggerCell.setPosition(point);

    //calibrate
    if (isNose) {
      calibrationNose_->calibrateInGeV(triggerCell);
    } else if (isSilicon) {
      if (isEM) {
        calibrationEE_->calibrateInGeV(triggerCell);
      } else {
        calibrationHEsi_->calibrateInGeV(triggerCell);
      }
    } else {
      calibrationHEsc_->calibrateInGeV(triggerCell);
    }//end calibrate
    trigCellVecOutput.push_back(triggerCell); 
  }//end for each trigger cell

  const std::array<float, latentSize> ECON_wafer = ECONout()->at(moduleID);
  id = HGCalTriggerDetId(subdet, zp, type, layer, waferU, waferV, 0, 0);
  for (unsigned iDim=0; iDim<latentSize; ++iDim){
    ae_encodedLayer_Output.emplace_back(ECON_wafer[iDim], iDim, id); 
  }
}//end select
