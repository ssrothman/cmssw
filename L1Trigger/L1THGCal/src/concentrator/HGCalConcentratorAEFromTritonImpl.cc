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
  inType_ = inputTypeStrToEnum(conf.getParameter<std::string>("inputType"));
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

  //get calibration factors fro first tc
  //note that we calibrate by absolute quantities, not transverse ones
  //as each tc in the wafer has different eta, but same calibration (I hope)
  double hwPtByMip = tc0.hwPt()/(tc0.mipPt()*cosh(tc0.eta()));
  double hwPtByEnergy = tc0.hwPt()/tc0.energy();

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

    const float AEout = AE_wafer[iTC];

    if (AEout<=0)
      continue;
  
    GlobalPoint point = geometry_->getTriggerCellPosition(id());

    //AE is predicting transverse charge. Proper charge is AEpt*cosh(eta)
    double AEQ_f = -1.;
    double AEmipPt = -1.;
    double AEet = -1.;
    switch (inType_){
      case ADC:
        AEQ_f = AEout;
        break;
      case ADCT:
        AEQ_f = AEout * cosh(point.eta());
        break;
      case MIP:
        //AEQ_f = AEout * hwPtByMip;
        AEmipPt = AEout / cosh(point.eta());
        break;
      case MIPT:
        //AEQ_f = AEout * hwPtByMip * cosh(point.eta());
        AEmipPt = AEout;
        break;
      case E:
        //AEQ_f = AEout * hwPtByEnergy;
        AEet = AEout / cosh(point.eta());
        break;
      case ET:
        //AEQ_f = AEout * hwPtByEnergy * cosh(point.eta());
        AEet = AEout;
        break;
      default:
        throw cms::Exception("HGCalConcentratorAEFromTritonImpl") << "Invalid InputType";
    }

    //round to int, and clip to >= 1
    int AEQ = std::max(static_cast<int>(std::round(AEQ_f)), 1);
    //int AEQ = std::max(static_cast<int>(AEQ_f), 1);
     
    //build 4-vector
    math::PtEtaPhiMLorentzVector p4(AEQ/cosh(point.eta()), point.eta(), point.phi(), 0.);

    //build tc object
    l1t::HGCalTriggerCell triggerCell(reco::LeafCandidate::LorentzVector(), AEQ, 0, 0, 0, id());
    //I hope these variables are not too important
    triggerCell.setCompressedCharge(999);
    triggerCell.setUncompressedCharge(999);
    triggerCell.setP4(p4);
    triggerCell.setPosition(point);

    //calibrate
    const HGCalTriggerCellCalibration* calibrator;
    if (isNose) {
      calibrator = calibrationNose_.get();
    } else if (isSilicon) {
      if (isEM) {
        calibrator = calibrationEE_.get();
      } else {
        calibrator = calibrationHEsi_.get();
      }
    } else {
      calibrator = calibrationHEsc_.get();
    }//end get calibrator

    if(AEQ_f>0){//if we autoencoded in ADCs then calibrate to MIPs
      calibrator->calibrateInMipT(triggerCell);
    } else if(AEmipPt>0){ //if we autoencoded in MIPs
      triggerCell.setMipPt(AEmipPt);
    } else { //if we autoencoded in GeV
      triggerCell.setMipPt(-1.);
    }

    if(AEet>0){//if we autoencoded in GeV, just set the energy
      math::PtEtaPhiMLorentzVector calibP4(AEet, point.eta(), point.phi(), 0.);
      triggerCell.setP4(calibP4);
    } else {//else calibrate the MipT energy to GeV
      calibrator->calibrateMipTinGeV(triggerCell);
    }

    trigCellVecOutput.push_back(triggerCell); 
  }//end for each trigger cell

  const std::array<float, latentSize> ECON_wafer = ECONout()->at(moduleID);

  id = HGCalTriggerDetId(subdet, zp, type, layer, waferU, waferV, 0, 0);
  for (unsigned iDim=0; iDim<latentSize; ++iDim){
    ae_encodedLayer_Output.emplace_back(ECON_wafer[iDim], iDim, id); 
  }
}//end select
