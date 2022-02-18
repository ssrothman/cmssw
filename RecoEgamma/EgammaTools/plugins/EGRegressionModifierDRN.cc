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

  bool patPho_;
  edm::InputTag patPhoSource_;
  edm::EDGetTokenT<edm::View<pat::Photon>> patPhoToken_;
  edm::View<pat::Photon> patPhotons_;

  bool gedPho_;
  edm::InputTag gedPhoSource_;
  edm::EDGetTokenT<edm::View<reco::Photon>> gedPhoToken_;
  edm::View<reco::Photon> gedPhotons_;

  bool patEle_;
  edm::InputTag patEleSource_;
  edm::EDGetTokenT<edm::View<pat::Electron>> patEleToken_;
  edm::View<pat::Electron> patElectrons_;

  bool gsfEle_;
  edm::InputTag gsfEleSource_;
  edm::EDGetTokenT<edm::View<reco::GsfElectron>> gsfEleToken_;
  edm::View<reco::GsfElectron> gsfElectrons_;

  edm::ValueMap<std::pair<float, float>> corrections_;

  template <typename T>
  std::pair<float, float> getCorrection_(T& part, const edm::View<T>& evtParts) const; 
};

EGRegressionModifierDRN::EGRegressionModifierDRN(const edm::ParameterSet& conf, edm::ConsumesCollector& cc)
    : ModifyObjectValueBase(conf),
      correctionsSource_{conf.getParameter<edm::InputTag>("correctionsSource")},
      correctionsToken_(cc.consumes<edm::ValueMap<std::pair<float, float>>>(correctionsSource_)){

    if (conf.exists("patPhotons")){
      patPhoSource_ = conf.getParameter<edm::InputTag>("patPhotons");
      patPhoToken_ = cc.consumes<edm::View<pat::Photon>>(patPhoSource_);
      patPho_ = true;
    } else {
      patPho_ = false;
    }

    if (conf.exists("gedPhotons")){
      gedPhoSource_ = conf.getParameter<edm::InputTag>("gedPhotons");
      gedPhoToken_ = cc.consumes<edm::View<reco::Photon>>(gedPhoSource_);
      gedPho_ = true;
    } else {
      gedPho_ = false;
    }

    if (conf.exists("patElectrons")){
      patEleSource_ = conf.getParameter<edm::InputTag>("patElectrons");
      patEleToken_ = cc.consumes<edm::View<pat::Electron>>(patEleSource_);
      patEle_ = true;
    } else {
      patEle_ = false;
    }
    
    if (conf.exists("gsfElectrons")){
      gsfEleSource_ = conf.getParameter<edm::InputTag>("gsfElectrons");
      gsfEleToken_ = cc.consumes<edm::View<reco::GsfElectron>>(gsfEleSource_);
      gsfEle_ = true;
    } else {
      gsfEle_ = false;
    }
}

EGRegressionModifierDRN::~EGRegressionModifierDRN() {}

void EGRegressionModifierDRN::setEvent(const edm::Event& evt) {
  corrections_ = evt.get(correctionsToken_);

  if (patEle_)
    patElectrons_ = evt.get(patEleToken_);

  if (gsfEle_)
    gsfElectrons_ = evt.get(gsfEleToken_);

  if (patPho_)
    patPhotons_ = evt.get(patPhoToken_);

  if (gedPho_)
    gedPhotons_ = evt.get(gedPhoToken_);
}

void EGRegressionModifierDRN::setEventContent(const edm::EventSetup& iSetup) {}

void EGRegressionModifierDRN::modifyObject(reco::GsfElectron& ele) const {
  std::pair<float, float> correction = getCorrection_(ele, gsfElectrons_);

  if(correction.first < 0)
    return;

  ele.setCorrectedEcalEnergy(correction.first, true);
  ele.setCorrectedEcalEnergyError(correction.second);

  std::pair<float, float> trackerCombo(1.0, 1.0); //compute E/p combination
  const math::XYZTLorentzVector newP4 = ele.p4() * trackerCombo.first / ele.p4().t();
  ele.correctMomentum(newP4, ele.trackMomentumError(), trackerCombo.second);

}

void EGRegressionModifierDRN::modifyObject(pat::Electron& ele) const {
  std::pair<float, float> correction = getCorrection_(ele, patElectrons_);

  if(correction.first < 0)
    return;

  ele.setCorrectedEcalEnergy(correction.first, true);
  ele.setCorrectedEcalEnergyError(correction.second);

  std::pair<float, float> trackerCombo(1.0, 1.0); //compute E/p combination
  const math::XYZTLorentzVector newP4 = ele.p4() * trackerCombo.first / ele.p4().t();
  ele.correctMomentum(newP4, ele.trackMomentumError(), trackerCombo.second);
}

void EGRegressionModifierDRN::modifyObject(pat::Photon& pho) const {
  std::pair<float, float> correction = getCorrection_(pho, patPhotons_);

  if(correction.first < 0)//regression failed/missing for some reason
    return; //don't apply any correction

  pho.setCorrectedEnergy(pat::Photon::P4type::regression2, correction.first, correction.second, true);
}

void EGRegressionModifierDRN::modifyObject(reco::Photon& pho) const {
  std::pair<float, float> correction = getCorrection_(pho, gedPhotons_);

  if(correction.first < 0)
    return;

  pho.setCorrectedEnergy(reco::Photon::P4type::regression2, correction.first, correction.second, true);
};

template <typename T>
std::pair<float, float> EGRegressionModifierDRN::getCorrection_(T& part, const edm::View<T>& evtParts) const{
  math::XYZTLorentzVectorD partP4 = part.p4();
  //math::XYZTLorentzVectorD partP4 = part.p4(T::P4type::ecal_standard);

  bool matched = false;
  unsigned i;
  for (i = 0; i < evtParts.size(); ++i) {
    T partIter = evtParts.at(i);
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

  edm::Ptr<T> ptr = evtParts.ptrAt(i);

  std::pair<float, float> correction = corrections_[ptr];

  std::cout << "Corrected energy is " << correction.first 
            << " +- " << correction.second << " GeV" 
            << "(eta = " << part.superCluster()->eta() << ")"
            << std::endl;

  return correction;
}

DEFINE_EDM_PLUGIN(ModifyObjectValueFactory, EGRegressionModifierDRN, "EGRegressionModifierDRN");
