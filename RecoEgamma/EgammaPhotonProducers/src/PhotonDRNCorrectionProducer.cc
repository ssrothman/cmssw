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
  const edm::InputTag photonProducer_;
  edm::EDGetTokenT<edm::View<reco::PhotonCore>> photonCoreToken_;
  edm::Handle<edm::View<reco::PhotonCore>> photonCoreHandle_;

  std::vector<std::pair<float,float>> corrections_;

};

PhotonDRNCorrectionProducer::PhotonDRNCorrectionProducer(const edm::ParameterSet& iConfig)
    : TritonEDProducer<>(iConfig, "PhotonDRNCorrectionProducer"),
      photonProducer_{iConfig.getParameter<edm::InputTag>("photonProducer")},
      photonCoreToken_(consumes(photonProducer_)) {

          produces<edm::ValueMap<std::pair<float,float>>>("DRNCorrection");
}

void PhotonDRNCorrectionProducer::beginLuminosityBlock(const edm::LuminosityBlock& iLumi,
                                                        const edm::EventSetup& iSetup) {
}

void PhotonDRNCorrectionProducer::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) {

    photonCoreHandle_ = iEvent.getHandle(photonCoreToken_); 

    if(!photonCoreHandle_.isValid()){
        throw cms::Exception("PhotonDRNCorrectionProducer") 
            << "Error! Cant get the photonCoreProducert " 
            << photonProducer_.label() << std::endl;
    }

    //clear previous output
    corrections_.clear();
    corrections_.reserve(photonCoreHandle_->size());

    if (photonCoreHandle_->empty()){
        client_->setBatchSize(0);
        return;
    } else {
        client_->setBatchSize(0); //TEMPORARY: don't run anything in Triton yet
    }

    //setup server input...
}

void PhotonDRNCorrectionProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) {
    if (photonCoreHandle_->empty())
        return;

    for (const auto& pho: *photonCoreHandle_){
        corrections_.emplace_back(std::pair<float,float>(1.0f,2.0f)); //TEMPORARY OUTPUT
    }

    //fill
    auto out = std::make_unique<edm::ValueMap<std::pair<float,float>>>();
    edm::ValueMap<std::pair<float,float>>::Filler filler(*out);
    filler.insert(photonCoreHandle_, corrections_.begin(), corrections_.end());

    iEvent.put(std::move(out), "DRNCorrection");
}

void PhotonDRNCorrectionProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("photonProducer");
  descriptions.add("PhotonDRNCorrectionProducer", desc);
}

DEFINE_FWK_MODULE(PhotonDRNCorrectionProducer);
