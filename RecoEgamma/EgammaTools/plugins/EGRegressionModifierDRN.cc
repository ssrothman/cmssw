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
  template <typename T>
  struct partVars {
    edm::InputTag source;
    edm::EDGetTokenT<edm::View<T>> token; 
    edm::View<T> particles;
    
    edm::InputTag correctionsSource;
    edm::EDGetTokenT<edm::ValueMap<std::pair<float, float>>> correctionsToken;
    edm::ValueMap<std::pair<float, float>> corrections;

    partVars(const edm::ParameterSet& config, edm::ConsumesCollector& cc){
      source = config.getParameter<edm::InputTag>("source");
      token = cc.consumes(source);

      correctionsSource = config.getParameter<edm::InputTag>("correctionsSource");
      correctionsToken = cc.consumes(correctionsSource);
    }

    const std::pair<float, float> getCorrection(T& part) const;
  };

  std::unique_ptr<partVars<pat::Photon>> patPhotons_;
  std::unique_ptr<partVars<pat::Electron>> patElectrons_;
  std::unique_ptr<partVars<reco::Photon>> gedPhotons_;
  std::unique_ptr<partVars<reco::GsfElectron>> gsfElectrons_;
};

EGRegressionModifierDRN::EGRegressionModifierDRN(const edm::ParameterSet& conf, edm::ConsumesCollector& cc)
    : ModifyObjectValueBase(conf){

    if (conf.exists("patPhotons")){
      patPhotons_ = std::make_unique<partVars<pat::Photon>>(conf.getParameterSet("patPhotons"), cc);
    }

    if (conf.exists("gedPhotons")){
      gedPhotons_ = std::make_unique<partVars<reco::Photon>>(conf.getParameterSet("gedPhotons"), cc);
    }

    if (conf.exists("patElectrons")){
      patElectrons_ = std::make_unique<partVars<pat::Electron>>(conf.getParameterSet("patElectrons"), cc);
    }

    if (conf.exists("gsfElectrons")){
      gsfElectrons_ = std::make_unique<partVars<reco::GsfElectron>>(conf.getParameterSet("gsfElectrons"), cc);
    }
}

EGRegressionModifierDRN::~EGRegressionModifierDRN() {}

void EGRegressionModifierDRN::setEvent(const edm::Event& evt) {

  if (patElectrons_){
    patElectrons_->particles = evt.get(patElectrons_->token);
    patElectrons_->corrections = evt.get(patElectrons_->correctionsToken);
  }

  if (patPhotons_){
    patPhotons_->particles = evt.get(patPhotons_->token);
    patPhotons_->corrections = evt.get(patPhotons_->correctionsToken);
  }

  if (gsfElectrons_){
    gsfElectrons_->particles = evt.get(gsfElectrons_->token);
    gsfElectrons_->corrections = evt.get(gsfElectrons_->correctionsToken);
  }

  if (gedPhotons_){
    gedPhotons_->particles = evt.get(gedPhotons_->token);
    gedPhotons_->corrections = evt.get(gedPhotons_->correctionsToken);
  }
}

void EGRegressionModifierDRN::setEventContent(const edm::EventSetup& iSetup) {}

void EGRegressionModifierDRN::modifyObject(reco::GsfElectron& ele) const {
  if(!gsfElectrons_)
    return;

  std::pair<float, float> correction = gsfElectrons_->getCorrection(ele);

  if(correction.first < 0)
    return;

  ele.setCorrectedEcalEnergy(correction.first, true);
  ele.setCorrectedEcalEnergyError(correction.second);

  std::pair<float, float> trackerCombo(1.0, 1.0); //compute E/p combination
  const math::XYZTLorentzVector newP4 = ele.p4() * trackerCombo.first / ele.p4().t();
  ele.correctMomentum(newP4, ele.trackMomentumError(), trackerCombo.second);
}

void EGRegressionModifierDRN::modifyObject(pat::Electron& ele) const {
  if(!patElectrons_)
    return;

  std::pair<float, float> correction = patElectrons_->getCorrection(ele);

  if(correction.first < 0)
    return;

  ele.setCorrectedEcalEnergy(correction.first, true);
  ele.setCorrectedEcalEnergyError(correction.second);

  std::pair<float, float> trackerCombo(1.0, 1.0); //compute E/p combination
  const math::XYZTLorentzVector newP4 = ele.p4() * trackerCombo.first / ele.p4().t();
  ele.correctMomentum(newP4, ele.trackMomentumError(), trackerCombo.second);
}

void EGRegressionModifierDRN::modifyObject(pat::Photon& pho) const {
  if(!patPhotons_)
    return;
  std::pair<float, float> correction = patPhotons_->getCorrection(pho);

  if(correction.first < 0)//regression failed/missing for some reason
    return; //don't apply any correction

  pho.setCorrectedEnergy(pat::Photon::P4type::regression2, correction.first, correction.second, true);
}

void EGRegressionModifierDRN::modifyObject(reco::Photon& pho) const {
  if(!gedPhotons_)
    return;

  std::pair<float, float> correction = gedPhotons_->getCorrection(pho);

  if(correction.first < 0)
    return;

  pho.setCorrectedEnergy(reco::Photon::P4type::regression2, correction.first, correction.second, true);
};

template <typename T>
const std::pair<float, float> EGRegressionModifierDRN::partVars<T>::getCorrection(T& part) const{
  math::XYZTLorentzVectorD partP4 = part.p4();
  //math::XYZTLorentzVectorD partP4 = part.p4(T::P4type::ecal_standard);

  bool matched = false;
  unsigned i;
  for (i = 0; i < particles.size(); ++i) {
    T partIter = particles.at(i);
    //math::XYZTLorentzVectorD p4Iter = partIter.p4(T::P4type::ecal_standard);
    math::XYZTLorentzVectorD p4Iter = partIter.p4();
    if (p4Iter == partP4) {
      matched = true;
      break;
    }
  }

  if (!matched) {
    throw cms::Exception("EGRegressionModifierDRN") 
      << "Matching failed in EGRegressionModifierDRN" << std::endl
      << "This should not have been possible" << std::endl;
    return std::pair<float,float>(-1., -1.);
  }

  edm::Ptr<T> ptr = particles.ptrAt(i);

  std::pair<float, float> correction = corrections[ptr];

  std::cout << "Corrected energy is " << correction.first 
            << " +- " << correction.second << " GeV" 
            << "(eta = " << part.superCluster()->eta() << ")"
            << std::endl;

  return correction;
}

DEFINE_EDM_PLUGIN(ModifyObjectValueFactory, EGRegressionModifierDRN, "EGRegressionModifierDRN");
