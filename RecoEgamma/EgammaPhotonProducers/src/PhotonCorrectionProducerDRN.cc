#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"
#include "HeterogeneousCore/SonicTriton/interface/TritonData.h"

#include "RecoEgamma/EgammaTools/interface/DRNRegressionHelper.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include <iostream>

/*
 * Compute photon object energy corrections using SonicTriton service and DynamicReductionNetwork
 * Produces ValueMap
 * This list of energy corrections is consumed by RecoEgamma/EgammaTools/EGRegressionModifierDRN
 *      in the GEDPhotonProducer flow to place the corrected energies in the GEDPhoton objects
 *
 * Author: Simon Rothman, MIT, 2022
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
    //DRNRegressionHelper helper_;

    EDGetTokenT<View<reco::PhotonCollection>> photonsToken_;
    Handle<View<reco::PhotonCollection>> photonsHandle_;
};

PhotonCorrectionProducerDRN::PhotonCorrectionProducerDRN(const ParameterSet& config)
    : TritonEDProducer<>(config, "PhotonCorrectionProducerDRN"),
      photonsToken_(consumes<reco::PhotonCollection>(config.getParameter<InputTag>("inputPhotons"))) //, other members
{
    prorudces<ValueMap<std::pair<float, float>>>(); 
}

void PhotonCorrectionProducerDRN::beginLuminosityBlock(const LuminosityBlock& lumi,
                                                       const EventSetup& setup){
    //propagate eventsetup
}

void PhotonCorrectionProducerDRN::acquire(const Event& event, const EventSetup& setup, Input& input){
    //setup input to server
    //event.getByToken(photonsToken_, photonsHandle_);

    //helper_.makeInput(photonsToken_, input) //TODO: not sure how tokens work
    std::cout << "acquiring" << std::endl;
    client_.setBatchSize(0);
}

void PhotonCorrectionProducerDRN::produce(const Event& event, const EventSetup& setup, 
                                          const Output& output){
    //recieve server output
    //<TYPE> regOutput = <initialize empty>; //TODO
    //helper_.getOutput(&RegOutput, output);

    std::cout << "producing" << std::endl;
    auto map = std::make_unique<ValueMap<std::pair<float, float>>>();
    ValueMap<std::pair<float, float>>::Filler filler(*map);

    event.getByToken(photonsToken_, photonsHandle_);

    unsigned nPho = photonsHandle->size();
    std::cout << "there are " << nPho << "photons" << std::endl;

    std::vector<std::pair<float, float>> values(nPho);

    for (unsigned i = 0; i < nPho; ++i){
        values[i] = std::pair<float,float>(1.0, 1.0);
    }

    filler.insert(photonsToken, values.begin(), values.end());
    filler.fill();
    event.put(std::move(map));
}

void PhotonCorrectionProducerDRN::fillDescriptions(ConfigurationDesecriptions& descriptions){
    //fill descriptions
}
