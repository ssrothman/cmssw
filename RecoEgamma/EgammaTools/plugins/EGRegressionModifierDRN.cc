#include "CommonTools/CandAlgos/interface/ModifyObjectValueBase.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "RecoEgamma/EgammaTools/interface/EgammaRegressionContainer.h"
#include "RecoEgamma/EgammaTools/interface/EpCombinationTool.h"
#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include <vdt/vdtMath.h>

/*
 * Same interface as EGRegressionModifierVX
 * Despite the fact that it doesn't actual do any computation, and some of the methods are just noops
 */

class EGRegressionModifierDRN : public ModifyObjectValueBase {
public:
  struct EleRegs {
    EleRegs(const edm::ParameterSet& iConfig, edm::ConsumesCollector& cc);
    void setEventContent(const edm::EventSetup& iSetup);
    EgammaRegressionContainer ecalOnlyMean;
    EgammaRegressionContainer ecalOnlySigma;
    EpCombinationTool epComb;
  };

  struct PhoRegs {
    PhoRegs(const edm::ParameterSet& iConfig, edm::ConsumesCollector& cc);
    void setEventContent(const edm::EventSetup& iSetup);
    EgammaRegressionContainer ecalOnlyMean;
    EgammaRegressionContainer ecalOnlySigma;
  };

  EGRegressionModifierDRN(const edm::ParameterSet& conf, edm::ConsumesCollector& cc);
  ~EGRegressionModifierDRN() override;

  void setEvent(const edm::Event&) final;
  void setEventContent(const edm::EventSetup&) final;

  void modifyObject(reco::GsfElectron&) const final;
  void modifyObject(reco::Photon&) const final;

  void modifyObject(pat::Electron&) const final;
  void modifyObject(pat::Photon&) const final;

private:
  //edm::Event event_;

  edm::InputTag correctionsSource_;
  edm::EDGetTokenT<edm::ValueMap<std::pair<float, float>>> correctionsToken_;

  edm::InputTag photonsSource_;
  edm::EDGetTokenT<edm::View<pat::Photon>> photonsToken_;

  edm::ValueMap<std::pair<float, float>> corrections_;
  edm::View<pat::Photon> photons_;
};

EGRegressionModifierDRN::EGRegressionModifierDRN(const edm::ParameterSet& conf, edm::ConsumesCollector& cc)
    : ModifyObjectValueBase(conf),
      correctionsSource_{conf.getParameter<edm::InputTag>("correctionsSource")},
      correctionsToken_(cc.consumes<edm::ValueMap<std::pair<float, float>>>(correctionsSource_)),
      photonsSource_{conf.getParameter<edm::InputTag>("photonsSource")},
      photonsToken_(cc.consumes<edm::View<pat::Photon>>(photonsSource_)) {}

EGRegressionModifierDRN::~EGRegressionModifierDRN() {}

void EGRegressionModifierDRN::setEvent(const edm::Event& evt) {
  corrections_ = evt.get(correctionsToken_);
  photons_ = evt.get(photonsToken_);
}

void EGRegressionModifierDRN::setEventContent(const edm::EventSetup& iSetup) {}

void EGRegressionModifierDRN::modifyObject(reco::GsfElectron& ele) const {
  //check if we have specified an electron regression correction and
  //return the object unmodified if so
}

void EGRegressionModifierDRN::modifyObject(pat::Electron& ele) const {
  modifyObject(static_cast<reco::GsfElectron&>(ele));
}

typedef math::XYZTLorentzVectorD LorentzVector;

void EGRegressionModifierDRN::modifyObject(pat::Photon& pho) const {
  //To get a ptr to index the ValueMap, we  
  //loop through all photons and find the right one
  //
  //There must be a better way...
  LorentzVector phoP4 = pho.p4(reco::Photon::P4type::ecal_standard);

  bool matched = false;
  unsigned i;
  for (i = 0; i < photons_.size(); ++i) {
    reco::Photon phoIter = photons_[i];
    LorentzVector p4Iter = phoIter.p4(reco::Photon::P4type::ecal_standard);
    if (p4Iter == phoP4) {
      matched = true;
      break;
    }
  }

  if (!matched) {
    throw cms::Exception("EGRegressionModifierDRN") 
      << "Matching failed in EGRegressionModifierDRN" << std::endl
      << "This should not have been possible" << std::endl;
    return;
  }

  edm::Ptr<pat::Photon> ptr = photons_.ptrAt(i);

  std::pair<float, float> correction = corrections_[ptr];

  if(correction.first < 0){//regression failed/missing for some reason
    return; //don't apply any correction
  }

  std::cout << "Corrected energy is " << correction.first 
            << " +- " << correction.second << " GeV" << std::endl;

  pho.setCorrectedEnergy(reco::Photon::P4type::regression2, correction.first, correction.second, true);
}

void EGRegressionModifierDRN::modifyObject(reco::Photon& pho) const {};

DEFINE_EDM_PLUGIN(ModifyObjectValueFactory, EGRegressionModifierDRN, "EGRegressionModifierDRN");
