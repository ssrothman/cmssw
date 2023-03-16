#ifndef __L1Trigger_L1THGCal_HGCalConcentratorAEFromTritonImpl_h__
#define __L1Trigger_L1THGCal_HGCalConcentratorAEFromTritonImpl_h__

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalConcentratorData.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include <vector>

#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

#include "L1Trigger/L1THGCal/interface/AEutil.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"

#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerCellCalibration.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"

#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalConcentratorData.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

class HGCalConcentratorAEFromTritonImpl {
public:
  HGCalConcentratorAEFromTritonImpl(const edm::ParameterSet& conf);

  void select(unsigned moduleID,
              const l1t::HGCalTriggerCell& tc0,
              std::vector<l1t::HGCalTriggerCell>& trigCellVecOutput,
              std::vector<l1t::HGCalConcentratorData>& ae_EncodedOutput);

  void setGeometry(const HGCalTriggerGeometryBase* const geom);

  void setAE(const AEMap* AEout, const ECONMap* ECONout) {
    AEout_ = AEout;
    ECONout_ = ECONout;
  }

  const AEMap* AEout(){
    if(!AEout_){
      throw cms::Exception("HGCalConcentratorProcessorSelection") 
        << " AEout not set ";
    } else {
      return AEout_;
    }
  }

  const ECONMap* ECONout(){
    if(!ECONout_){
      throw cms::Exception("HGCalConcentratorProcessorSelection")
        << "ECONout not set";
    } else {
      return ECONout_;
    }
  }

private:
  const AEMap * AEout_;
  const ECONMap * ECONout_;

  std::unique_ptr<HGCalTriggerCellCalibration> calibrationEE_;
  std::unique_ptr<HGCalTriggerCellCalibration> calibrationHEsi_;
  std::unique_ptr<HGCalTriggerCellCalibration> calibrationHEsc_;
  std::unique_ptr<HGCalTriggerCellCalibration> calibrationNose_;

  HGCalTriggerTools triggerTools_;

  const HGCalTriggerGeometryBase* geometry_;

  InputType inType_;
};

#endif
