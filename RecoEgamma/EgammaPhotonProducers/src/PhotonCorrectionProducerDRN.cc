#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"
#include "HeterogeneousCore/SonicTriton/interface/TritonData.h"

#include "RecoEgamma/EgammaTools/DRNRegressionHelper.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/*
 * Compute photon object energy corrections using SonicTriton service and DynamicReductionNetwork
 * Producers list of energy corrections and uncertainties
 * This list of energy corrections is consumed by RecoEgamma/EgammaTools/EGRegressionModifierDRN
 *      in the GEDPhotonProducer flow to place the corrected energies in the GEDPhoton objects
 *
 * Author: Simon Rothman, MIT, 2021
 */


using namespace edm;

class PhotonCorrectionProducerDRN : public TritonEDProducer<> {
public:
    explicit PhotonCorrectionProducerDRN(const ParameterSet& config);

    void beginLuminosityBlock(const LuminosityBlock& lumi, const EventSetup& setup) override;

    void acquire(const Event& event, const EventSetup& setup, Input& input) override;
    void produce(const Event& event, const EventSetup& setup, const Output& output) override;

    static void fillDescriptions(ConfigurationDescriptions& descriptions);

private:
    //private members
    DRNRegressionHelper helper_;
};

PhotonCorrectionProducerDRN::PhotonCorrectionProducerDRN(const ParameterSet& config)
    : TritonEDProducer<>(config, "PhotonCorrectionProducerDRN") //, other members
{
    prorudcer<TYPE>()
}

void PhotonCorrectionProducerDRN::beginLuminosityBlock(const LuminosityBlock& lumi,
                                                       const EventSetup& setup){
    //propagate eventsetup
}

void PhotonCorrectionProducerDRN::acquire(const Event& event, const EventSetup& setup, Input& input){
    //setup input to server
}

void PhotonCorrectionProducerDRN::produce(const Event& event, const EventSetup& setup, 
                                          const Output& output){
    //recieve server output
}

void PhotonCorrectionProducerDRN::fillDescriptions(ConfigurationDesecriptions& descriptions){
    //fill descriptions
}
