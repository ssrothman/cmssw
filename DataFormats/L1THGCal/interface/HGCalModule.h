#ifndef DataFormats_L1TCalorimeter_HGCalModule_h
#define DataFormats_L1TCalorimeter_HGCalModule_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"

namespace l1t {
class HGCalModule {
public:
    HGCalModule(uint32_t moduleid, uint32_t tcid0,
               float energy, float mipPt,
               const std::vector<l1t::HGCalTriggerCell>& tc_indices,
               const std::vector<l1t::HGCalTriggerCellTruth>& tc_truths)
        : moduleid_(moduleid), tcid0_(tcid0),
          energy_(energy), mipPt_(mipPt),
          tc_indices_(tc_indices),
          tc_truths_(tc_truths) {}

    HGCalModule() 
        : moduleid_(0), tcid0_(0),
          energy_(0), mipPt_(0),
          tc_indices_(),
          tc_truths_() {}

    uint32_t moduleId() const { return moduleid_; }
    uint32_t tcId0() const { return tcid0_; }
    float energy() const { return energy_; }
    float mipPt() const { return mipPt_; }

    const std::vector<l1t::HGCalTriggerCell>& tcs() const { return tc_indices_; }
    const std::vector<l1t::HGCalTriggerCellTruth>& tcTruths() const { return tc_truths_; }
private:
    uint32_t moduleid_;
    uint32_t tcid0_;

    float energy_;
    float mipPt_;

    std::vector<l1t::HGCalTriggerCell> tc_indices_;
    std::vector<l1t::HGCalTriggerCellTruth> tc_truths_;
};

class HGCalECONdata {
public:
    HGCalECONdata(int sumCALQ, 
                  const std::vector<int>& ae_norm,
                  const std::vector<int>& ae_ADC,
                  const std::vector<int>& ae_CALQ,
                  const std::vector<float>& ae_input)
        : sumCALQ(sumCALQ), 
          ae_norm(ae_norm), 
          ae_ADC(ae_ADC), 
          ae_CALQ(ae_CALQ), 
          ae_input(ae_input) {}

    HGCalECONdata() 
        : sumCALQ(0), 
          ae_norm(), 
          ae_ADC(), 
          ae_CALQ(), 
          ae_input() {}

    int getSumCALQ() const { return sumCALQ; }
    const std::vector<int>& getAENorm() const { return ae_norm; }
    const std::vector<int>& getAEADC() const { return ae_ADC; }
    const std::vector<int>& getAECALQ() const { return ae_CALQ; }
    const std::vector<float>& getAEInput() const { return ae_input; }
private:
    int sumCALQ;
    std::vector<int> ae_norm;
    std::vector<int> ae_ADC;
    std::vector<int> ae_CALQ;
    std::vector<float> ae_input;    
};

};
#endif
