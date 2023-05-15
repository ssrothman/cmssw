#ifndef AEINPUTUTIL_H
#define AEINPUTUTIL_H

#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"

class AEinputUtil{
public:
    AEinputUtil(unsigned bitsPerADC, unsigned bitsPerNorm, unsigned bitsPerCALQ, unsigned bitsPerInput, 
                bool useModuleFactor, bool bitShiftNormalize, bool useTransverseADC, bool normByMax):
        bitsPerADC_(bitsPerADC),
        bitsPerNorm_(bitsPerNorm),
        bitsPerCALQ_(bitsPerCALQ),
        bitsPerInput_(bitsPerInput),
        useModuleFactor_(useModuleFactor),
        bitShiftNormalize_(bitShiftNormalize),
        useTransverseADC_(useTransverseADC),
        normByMax_(normByMax),
        maxNormInt_(1 << bitsPerNorm)
    {
        //pass
    }

    template <typename T>
    void print2d(const T arr[8][8]) const {
        for(unsigned u=0; u<8; ++u){
            for(unsigned v=0; v<8; ++v){
                std::cout << arr[u][v] << " ";
            }
            std::cout << std::endl;
        }
    }

    void setGeometry(const HGCalTriggerGeometryBase* const geom){
        triggerTools_.setGeometry(geom);
    }

    inline const HGCalTriggerGeometryBase* geometry() const {
        return triggerTools_.getTriggerGeometry();
    }

    void run(const std::vector<l1t::HGCalTriggerCell>& trigCellVecInput){
        //printf("top of run\n");
        //fflush(stdout);
        clear();
        //printf("cleared\n");
        //fflush(stdout);

        if(trigCellVecInput.empty()){
            return;
        }
        //printf("not empty\n");
        //fflush(stdout);

        unsigned module_det_id = geometry()->getModuleFromTriggerCell(trigCellVecInput[0].detId());
        setupNorms(module_det_id);
        //printf("set up norms\n");
        //fflush(stdout);

        fillADCs(trigCellVecInput);
        //printf("filled ADCs\n");
        //fflush(stdout);
        fillCALQs();
        //printf("filled CALQs\n");
        //fflush(stdout);
        fillInputs();
        //printf("filled inputs\n");
        //fflush(stdout);
        //printf("ADCs\n");
        //fflush(stdout);
        //print2d(ADCs_);
        //fflush(stdout);
        //printf("norms\n");
        //fflush(stdout);
        //print2d(norms_);
        //fflush(stdout);
        //printf("CALQs\n");
        //fflush(stdout);
        //print2d(CALQs_);
        //fflush(stdout);
        //printf("inputs\n");
        //fflush(stdout);
        //print2d(inputs_);
        //fflush(stdout);
        //printf("\n\n");
        //fflush(stdout);
    }

    inline unsigned getNorm(unsigned u, unsigned v) const{
        return norms_[u][v];
    }

    inline unsigned getADC(unsigned u, unsigned v) const{
        return ADCs_[u][v];
    }

    inline unsigned getCALQ(unsigned u, unsigned v) const{
        return CALQs_[u][v];
    }

    inline unsigned getInput(unsigned u, unsigned v) const{
        return inputs_[u][v];
    }

    inline size_t getModSum() const{
        return modSum_;
    }

    inline const std::vector<unsigned>& getUs() const{
        return us_;
    }

    inline const std::vector<unsigned>& getVs() const{
        return vs_;
    }

    inline double getInputNorm() const {
        return double(1 << (bitsPerInput_ - 1));
    }

    inline double CALQtoADC(double CALQ, unsigned u, unsigned v){
        return (CALQ / double(norms_[u][v])) * std::pow(2, bitsPerADC_ + bitsPerNorm_ - bitsPerCALQ_);
    }

private:
    template <typename T>
    unsigned msb(T x){
        unsigned msb = 0;
        while(x >>= 1){
            msb++;
        }
        return msb;
    }

    void fillADCs(const std::vector<l1t::HGCalTriggerCell>& trigCellVecInput){
        //printf("IRL\n");
        for(const auto& tc : trigCellVecInput){
            auto id = HGCalTriggerDetId(tc.detId());
            unsigned u = id.triggerCellU();
            unsigned v = id.triggerCellV();
            ADCs_[u][v] = tc.hwPt();
            us_.push_back(u);
            vs_.push_back(v);
        }
    }

    void fillCALQs(){
        for(unsigned u=0; u<cellUVSize_; u++){
            for(unsigned v=0; v<cellUVSize_; v++){
                size_t CALQ = ADCs_[u][v] * norms_[u][v];
                CALQs_[u][v] = CALQ >> (bitsPerADC_ + bitsPerNorm_ - bitsPerCALQ_); //truncate
                modSum_ += CALQs_[u][v];
            }
        }
    }

    void fillInputs(){
        //printf("In filling inputs\n");
        int shift;
        if(normByMax_){
            unsigned q=0;
            for(unsigned u=0; u<cellUVSize_; u++){
                for(unsigned v=0; v<cellUVSize_; v++){
                    q |= CALQs_[u][v];
                }
            }
            //printf("msb(q) = %u\n", msb(q));
            shift = msb(q) - bitsPerInput_ + 1; 
            //printf("shift = %d\n", shift);
        } else {
            //printf("msb(modSum_) = %u\n", msb(modSum_));
            shift = msb(modSum_) - bitsPerInput_ + 1;
            //printf("shift = %d\n", shift);
        }
        for(unsigned u=0; u<cellUVSize_; u++){
            for(unsigned v=0; v<cellUVSize_; v++){
                size_t CALQ = CALQs_[u][v];
                if(bitShiftNormalize_){
                    if(shift > 0){
                        CALQ = CALQ >> shift; //truncate
                    } else {
                        CALQ = CALQ << -shift;
                    }
                } else {
                    CALQ = size_t(getInputNorm() * CALQ / double(modSum_));//round
                }
                inputs_[u][v] = CALQ;
            }
        }
    }

    void setupNorms(unsigned module_det_id){
        if(!useTransverseADC_){//if not use transverse ADC, norms don't have to do anything
            for(unsigned i=0; i<cellUVSize_; i++){
                for(unsigned j=0; j<cellUVSize_; j++){
                    norms_[i][j] = maxNormInt_ >> 1;
                }
            }
        } else {//compute norms

            //printf("SETTING UP NORMS\n");
            //constituent triger cells
            const auto& trigger_cell_ids = geometry()->getTriggerCellsFromModule(module_det_id);

            moduleFactor_ = maxNormInt_ >> 1;
            if(useModuleFactor_){//if using module factor, find smallest cosh(eta)
                double minCosh = 1e9;
                for(const auto& tc_id : trigger_cell_ids){
                    const auto& pos = geometry()->getTriggerCellPosition(tc_id);
                    double val = std::cosh(pos.eta());
                    if(val < minCosh) {
                        minCosh = val;
                    }
                }
                moduleFactor_ = minCosh * moduleFactor_;
            }

            //printf("MODULE FACTOR %0.3lf\n", moduleFactor);
            for(const auto& tc_id : trigger_cell_ids){
                const auto& tc = HGCalTriggerDetId(tc_id);
                const auto& pos = geometry()->getTriggerCellPosition(tc_id);
                unsigned u = tc.triggerCellU();
                unsigned v = tc.triggerCellV();
                double norm = moduleFactor_/std::cosh(pos.eta()); //floating point number btw 0 and 1
                //printf("\tnorm %0.3lf\n", norm);
                //can round bc these are precomputed
                norms_[u][v] = std::round(norm); //12 bit int
                //printf("\tnorm[u][v] %u\n", norms_[u][v]);
            }
        }
    }


    void clear(){
        for(unsigned i=0; i<cellUVSize_; i++){
            for(unsigned j=0; j<cellUVSize_; j++){
                norms_[i][j] = 0;
                inputs_[i][j] = 0;
                CALQs_[i][j] = 0;
                ADCs_[i][j] = 0;
            }
        }
        us_.clear();
        vs_.clear();
        modSum_ = 0;
    }
    static constexpr unsigned cellUVSize_ = 8;
    static constexpr unsigned nTriggerCells_ = 48;

    unsigned bitsPerADC_;
    unsigned bitsPerNorm_;
    unsigned bitsPerCALQ_;
    unsigned bitsPerInput_;

    bool useModuleFactor_;
    bool bitShiftNormalize_;
    bool useTransverseADC_;
    bool normByMax_;

    size_t maxNormInt_ ;
    
    HGCalTriggerTools triggerTools_;

    unsigned ADCs_[cellUVSize_][cellUVSize_]; // 21 bit integers
    unsigned norms_[cellUVSize_][cellUVSize_]; // 12 bit integers
    unsigned CALQs_[cellUVSize_][cellUVSize_]; // 22 bit integers
    unsigned inputs_[cellUVSize_][cellUVSize_]; // 8 bit integers
    
    std::vector<unsigned> us_, vs_;
    size_t modSum_;
    double moduleFactor_;
};

#endif
