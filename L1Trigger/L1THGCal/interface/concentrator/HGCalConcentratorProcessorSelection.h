#ifndef __L1Trigger_L1THGCal_HGCalConcentratorProcessorSelection_h__
#define __L1Trigger_L1THGCal_HGCalConcentratorProcessorSelection_h__

#include "L1Trigger/L1THGCal/interface/HGCalProcessorBase.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorThresholdImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorBestChoiceImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorSuperTriggerCellImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorCoarsenerImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorTrigSumImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorAutoEncoderImpl.h"
#include "L1Trigger/L1THGCal/interface/concentrator/HGCalConcentratorAEFromTritonImpl.h"

#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerSums.h"
#include "DataFormats/L1THGCal/interface/HGCalConcentratorData.h"

#include "L1Trigger/L1THGCal/interface/AEutil.h"

#include <utility>
#include <tuple>

class HGCalConcentratorProcessorSelection : public HGCalConcentratorProcessorBase {
private:
  enum SelectionType { thresholdSelect, bestChoiceSelect, superTriggerCellSelect, autoEncoderSelect, AEFromTritonSelect, noSelection };

public:
  HGCalConcentratorProcessorSelection(const edm::ParameterSet& conf);

  void run(const edm::Handle<l1t::HGCalTriggerCellBxCollection>& triggerCellCollInput,
           std::tuple<l1t::HGCalTriggerCellBxCollection,
                      l1t::HGCalTriggerSumsBxCollection,
                      l1t::HGCalConcentratorDataBxCollection>& triggerCollOutput) override;


  void setAE(const AEMap *AEout, const ECONMap *ECONout) override {
    if(AEFromTritonImpl_){
      AEFromTritonImpl_->setAE(AEout, ECONout);
    }
  }

  bool wantsAE() override{
    return static_cast<bool>(AEFromTritonImpl_);
  }

private:
  bool fixedDataSizePerHGCROC_;
  bool allTrigCellsInTrigSums_;
  std::vector<unsigned> coarsenTriggerCells_;
  static constexpr int kHighDensityThickness_ = 0;
  static constexpr int kNSubDetectors_ = 3;

  std::vector<SelectionType> selectionType_;

  std::unique_ptr<HGCalConcentratorThresholdImpl> thresholdImpl_;
  std::unique_ptr<HGCalConcentratorBestChoiceImpl> bestChoiceImpl_;
  std::unique_ptr<HGCalConcentratorSuperTriggerCellImpl> superTriggerCellImpl_;
  std::unique_ptr<HGCalConcentratorCoarsenerImpl> coarsenerImpl_;
  std::unique_ptr<HGCalConcentratorTrigSumImpl> trigSumImpl_;
  std::unique_ptr<HGCalConcentratorAutoEncoderImpl> autoEncoderImpl_;
  std::unique_ptr<HGCalConcentratorAEFromTritonImpl> AEFromTritonImpl_;

  HGCalTriggerTools triggerTools_;
};

#endif
