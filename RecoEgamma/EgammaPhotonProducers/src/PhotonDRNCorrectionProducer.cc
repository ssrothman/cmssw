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

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
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
 * DRNCorrectionProducerT
 *
 * Simple producer to generate a ValueMap of corrected energies and resolutions for photons
 * I might generalize this to have one producer for photons and electrons if this looks possible
 *
 * Author: Simon Rothman (MIT)
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

template<typename T>
class DRNCorrectionProducerT : public TritonEDProducer<> {
public:
  explicit DRNCorrectionProducerT(const edm::ParameterSet& iConfig);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  void acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& input) override;
  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) override;

  void beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& iSetup) override;

private:
  const edm::InputTag particleSource_;
  edm::EDGetTokenT<edm::View<T>> particleToken_;
  edm::Handle<edm::View<T>> particles_;

  const edm::InputTag rhoName_;
  edm::EDGetTokenT<double> rhoToken_;
  float rho_;

  std::vector<std::pair<float,float>> corrections_;

  std::vector<float> HoEs_;

  size_t nPart_;

};

template<typename T>
DRNCorrectionProducerT<T>::DRNCorrectionProducerT(const edm::ParameterSet& iConfig)
    : TritonEDProducer<>(iConfig, "DRNCorrectionProducerT"),
      particleSource_{iConfig.getParameter<edm::InputTag>("particleSource")},
      particleToken_(consumes(particleSource_)),
      rhoName_{iConfig.getParameter<edm::InputTag>("rhoName")},
      rhoToken_(consumes(rhoName_)){

          produces<edm::ValueMap<std::pair<float,float>>>();
}

template<typename T>
void DRNCorrectionProducerT<T>::beginLuminosityBlock(const edm::LuminosityBlock& iLumi,
                                                        const edm::EventSetup& iSetup) {
}

template<typename T>
void DRNCorrectionProducerT<T>::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) {


    particles_ = iEvent.getHandle(particleToken_); 
    rho_ = iEvent.get(rhoToken_);

    nPart_ = particles_->size();

    corrections_.clear();
    corrections_.reserve(nPart_);

    HoEs_.clear();
    HoEs_.reserve(nPart_);

    if (nPart_==0){
        client_->setBatchSize(0);
        return;
    } else {
        client_->setBatchSize(0); //TEMPORARY: don't run anything in Triton yet
    }

    //setup server input...
    for(auto& pho : *particles_){
      HoEs_.push_back(pho.hadronicOverEm());
    }

}

template<typename T>
void DRNCorrectionProducerT<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) {

    particles_ = iEvent.getHandle(particleToken_); 

    for (unsigned i=0; i < nPart_; ++i){
        corrections_.emplace_back(std::pair<float,float>(1.0f,2.0f)); //TEMPORARY OUTPUT
    }

    //fill
    auto out = std::make_unique<edm::ValueMap<std::pair<float,float>>>();
    edm::ValueMap<std::pair<float,float>>::Filler filler(*out);
    filler.insert(particles_, corrections_.begin(), corrections_.end());
    filler.fill();

    iEvent.put(std::move(out));
}

template<typename T>
void DRNCorrectionProducerT<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("particleSource");
  desc.add<edm::InputTag>("rhoName");
  descriptions.addWithDefaultLabel(desc);
}

using PatPhotonDRNCorrectionProducer = DRNCorrectionProducerT<pat::Photon>;
using PhotonDRNCorrectionProducer = DRNCorrectionProducerT<reco::Photon>;
using GsfElectronDRNCorrectionProducer = DRNCorrectionProducerT<reco::GsfElectron>;
using PatElectronDRNCorrectionProducer = DRNCorrectionProducerT<pat::Electron>;

//DEFINE_FWK_MODULE(DRNCorrectionProducerT);
DEFINE_FWK_MODULE(PatPhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(PhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(GsfElectronDRNCorrectionProducer);
DEFINE_FWK_MODULE(PatElectronDRNCorrectionProducer);
