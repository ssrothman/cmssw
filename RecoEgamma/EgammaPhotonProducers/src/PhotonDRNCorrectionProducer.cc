#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"
#include "HeterogeneousCore/SonicTriton/interface/TritonData.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Handle.h"

#include <sstream>
#include <string>
#include <vector>
#include <random>

#include <iostream>

/*
 * PhotonDRNCorrectionProducer
 *
 * Simple producer to generate a set of corrected superclusters with the DRN regression
 * Based on RecoEcal/EgammaClusterProducers/SCEnergyCorrectorProducer by S. Harper (RAL/CERN)
 *
 * Author: Simon Rothman (UMN, MIT)
 *
 */

namespace {
  float sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }

  float logcorrection(float x) {
    static float ln2 = log(2);
    return ln2 * 2 * (sigmoid(x) - 0.5);
  }

  float correction(float x) { return exp(-logcorrection(x)); }
}  // namespace

class PhotonDRNCorrectionProducer : public TritonEDProducer<> {
public:
  explicit PhotonDRNCorrectionProducer(const edm::ParameterSet& iConfig);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


  void acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& input) override;
  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) override;

  void beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& iSetup) override;

private:
  const edm::InputTag photonSource_;

  edm::EDGetTokenT<edm::View<reco::Photon>> photonToken_;
  edm::Handle<edm::View<reco::Photon>> photonHandle_;

  std::vector<std::pair<float,float>> corrections_;

};

PhotonDRNCorrectionProducer::PhotonDRNCorrectionProducer(const edm::ParameterSet& iConfig)
    : TritonEDProducer<>(iConfig, "PhotonDRNCorrectionProducer"),
      photonSource_{iConfig.getParameter<edm::InputTag>("photonSource")},
      photonToken_(consumes(photonSource_)) {

          produces<edm::ValueMap<std::pair<float,float>>>("DRNCorrection");
}

void PhotonDRNCorrectionProducer::beginLuminosityBlock(const edm::LuminosityBlock& iLumi,
                                                        const edm::EventSetup& iSetup) {
}

void PhotonDRNCorrectionProducer::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) {

    //std::cout << "top of acquire" << std::endl;

    photonHandle_ = iEvent.getHandle(photonToken_); 

    if(!photonHandle_.isValid()){
        throw cms::Exception("PhotonDRNCorrectionProducer") 
            << "Error! Cant get the photons from " 
            << photonSource_.label() << std::endl;
    }

    std::cout << "there are " << photonHandle_->size() << " photons in the DRN Corrector" << std::endl;

    //throw cms::Exception("PhotonDRNCorrectionProducer") << "test" << std::endl;

    //clear previous output
    corrections_.clear();
    corrections_.reserve(photonHandle_->size());

    //std::cout << "setup corrections vector" << std::endl;

    if (photonHandle_->empty()){
        client_->setBatchSize(0);
        return;
    } else {
        client_->setBatchSize(0); //TEMPORARY: don't run anything in Triton yet
    }

    //setup server input...
    //std::cout << "End of acquire" << std::endl;
}

void PhotonDRNCorrectionProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) {

    //std::cout << "top of produce" << std::endl;

    photonHandle_ = iEvent.getHandle(photonToken_); 
    
    if (photonHandle_->empty())
        return;

    //std::cout << "photon handle is not empty" << std::endl;

    //for (const auto& pho: *photonHandle_){
    for (unsigned i=0; i<photonHandle_->size(); ++i){
        corrections_.emplace_back(std::pair<float,float>(1.0f,2.0f)); //TEMPORARY OUTPUT
        //std::cout << "emplacing" << i << "... " << std::endl;
        //std::cout << "photon :D" << std::endl;
    }

    //std::cout << "filled corrections" << std::endl;

    //fill
    auto out = std::make_unique<edm::ValueMap<std::pair<float,float>>>();
    edm::ValueMap<std::pair<float,float>>::Filler filler(*out);
    filler.insert(photonHandle_, corrections_.begin(), corrections_.end());

    //std::cout << "filled ValueMap" << std::endl;

    iEvent.put(std::move(out), "DRNCorrection");

    //std::cout << "bottom of produce" << std::endl;
}

void PhotonDRNCorrectionProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("photonSource");
  descriptions.add("PhotonDRNCorrectionProducer", desc);
}

DEFINE_FWK_MODULE(PhotonDRNCorrectionProducer);
