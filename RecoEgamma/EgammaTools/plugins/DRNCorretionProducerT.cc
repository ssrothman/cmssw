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
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"

#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"

#include <sstream>
#include <string>
#include <vector>
#include <random>

#include <iostream>

/*
 * DRNCorrectionProducerT
 *
 * Producer template generate a ValueMap of corrected energies and resolutions
 * ValueMap contains std::pair<float, float> of corrected energy resolution 
 *
 * Author: Simon Rothman (MIT)
 * Written 2022
 *
 */

namespace {
  float sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }

  float logcorrection(float x) {
    static float ln2 = log(2);
    return ln2 * 2 * (sigmoid(x) - 0.5);
  }

  //correction factor is transformed by sigmoid and "logratioflip target"
  float correction(float x) { return exp(-logcorrection(x)); }

  inline float rescale(float x, float min, float range){
    return (x-min)/range;
  }

  const float RHO_MIN = 0.0f;
  const float RHO_RANGE = 13.0f;

  const float HOE_MIN = 0.0f;
  const float HOE_RANGE = 0.05f;

  const float XY_MIN = -150.0f;
  const float XY_RANGE = 300.0f;

  const float Z_MIN = -330.0f;
  const float Z_RANGE = 660.0f;
  
  const float NOISE_MIN = 0.9f;
  const float NOISE_RANGE = 3.0f;

  const float ECAL_MIN = 0.0f;
  const float ECAL_RANGE = 250.0f;

  const float ES_MIN = 0.0f;
  const float ES_RANGE = 0.1f;

}  //anon namespace for private utility constants and functions

template <typename T>
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

  std::vector<std::pair<float, float>> corrections_;

  edm::InputTag EBRecHitsName_, EERecHitsName_, ESRecHitsName_;
  edm::EDGetTokenT<EcalRecHitCollection> EBRecHitsToken_, EERecHitsToken_, ESRecHitsToken_;
  edm::Handle<EcalRecHitCollection> EERecHits_, EBRecHits_, ESRecHits_;

  std::vector<float> HoEs_;

  size_t nPart_, nPartEE_, nPartEB_;

  edm::ESHandle<EcalPedestals> ped_;
  edm::ESHandle<CaloGeometry> pG_;

  bool isEB(const T& part);
  bool isEE(const T& part);
  bool skip(const T& part);
};

template <typename T>
DRNCorrectionProducerT<T>::DRNCorrectionProducerT(const edm::ParameterSet& iConfig)
    : TritonEDProducer<>(iConfig, "DRNCorrectionProducerT"),
      particleSource_{iConfig.getParameter<edm::InputTag>("particleSource")},
      particleToken_(consumes(particleSource_)),
      rhoName_{iConfig.getParameter<edm::InputTag>("rhoName")},
      rhoToken_(consumes(rhoName_)),
      EBRecHitsName_{iConfig.getParameter<edm::InputTag>("reducedEcalRecHitsEB")},
      EERecHitsName_{iConfig.getParameter<edm::InputTag>("reducedEcalRecHitsEE")},
      ESRecHitsName_{iConfig.getParameter<edm::InputTag>("reducedEcalRecHitsES")},
      EBRecHitsToken_(consumes<EcalRecHitCollection>(EBRecHitsName_)),
      EERecHitsToken_(consumes<EcalRecHitCollection>(EERecHitsName_)),
      ESRecHitsToken_(consumes<EcalRecHitCollection>(ESRecHitsName_))

{
  produces<edm::ValueMap<std::pair<float, float>>>();
}

template <typename T>
void DRNCorrectionProducerT<T>::beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& iSetup) {
}

template <typename T>
bool DRNCorrectionProducerT<T>::isEB(const T& part){
  return part.superCluster()->seed()->hitsAndFractions().at(0).first.subdetId() == EcalBarrel; 
}

template <typename T>
bool DRNCorrectionProducerT<T>::isEE(const T& part){
  return part.superCluster()->seed()->hitsAndFractions().at(0).first.subdetId() == EcalEndcap; 
}

template <typename T>
bool DRNCorrectionProducerT<T>::skip(const T& part){
  /*
   * Separated out from acquire() and produce() to ensure that skipping check is identical in both
   */
  return (!isEB(part) && !isEE(part)) || part.superCluster()->hitsAndFractions().empty();
}

template <typename T>
void DRNCorrectionProducerT<T>::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) {
  /*
   * Get products from event and event setup
   */
  particles_ = iEvent.getHandle(particleToken_);
  rho_ = iEvent.get(rhoToken_);
  EBRecHits_ = iEvent.getHandle(EBRecHitsToken_);
  EERecHits_ = iEvent.getHandle(EERecHitsToken_);
  ESRecHits_ = iEvent.getHandle(ESRecHitsToken_);

  iSetup.get<EcalPedestalsRcd>().get(ped_);
  iSetup.get<CaloGeometryRecord>().get(pG_);
  const CaloGeometry* geo = pG_.product();
  const CaloSubdetectorGeometry* ecalEBGeom =
      static_cast<const CaloSubdetectorGeometry*>(geo->getSubdetectorGeometry(DetId::Ecal, EcalBarrel));
  const CaloSubdetectorGeometry* ecalEEGeom =
      static_cast<const CaloSubdetectorGeometry*>(geo->getSubdetectorGeometry(DetId::Ecal, EcalEndcap));
  const CaloSubdetectorGeometry* ecalESGeom =
      static_cast<const CaloSubdetectorGeometry*>(geo->getSubdetectorGeometry(DetId::Ecal, EcalPreshower));

  const EcalRecHitCollection* recHitsEB = EBRecHits_.product();
  const EcalRecHitCollection* recHitsEE = EERecHits_.product();
  const EcalRecHitCollection* recHitsES = ESRecHits_.product();

  nPart_ = particles_->size();

  HoEs_.clear();
  HoEs_.reserve(nPart_);

  if (nPart_ == 0) {
    client_->setBatchSize(0);
    return;
  } else {
    client_->setBatchSize(1);
  }

  /*
   * Determine how many particles, how many RecHits there are in each subdetector
   * This seems a little awkward, as we have to loop through particles twice:
   *  once here to count them, and then again to fill the input tensors
   * 
   * N.B. in MiniAOD there are sometimes particles with no RecHits
   * We can not apply our regression to these, so we skip them
   */
  unsigned nHitsEB = 0, nHitsEE = 0, nHitsES = 0;
  nPartEB_ = 0;
  nPartEE_ = 0;
  bool EB = 0;
  for (auto& part : *particles_) {
    const reco::SuperClusterRef& sc = part.superCluster();
    EB = isEB(part);

    if(skip(part))
      continue;

    unsigned nHitsECAL = sc->hitsAndFractions().size();

    if(EB){
      nHitsEB += nHitsECAL;
      ++nPartEB_;
    } else {
      nHitsEE += nHitsECAL;
      ++nPartEE_;
    }

    for (auto iES = sc->preshowerClustersBegin(); iES != sc->preshowerClustersEnd(); ++iES) {
       nHitsES += (*iES)->hitsAndFractions().size();
    }
  }

  /*
   * Allocate DRN inputs ({SB} is one of EB, EE, ES):
   * x{SB}: (x, y, z, energy, [noise]) continuous-valued inputs per RecHit
   * f{SB}: (flagVal) integer denoting RecHit flag values
   * gain{SB}: (gain) integer in (0, 1, 2) denoting gain value
   * gx{SB}: (rho, H/E) additional high-level features.
   * batch{SB}: graph models require explicitely passing the particle index for each RecHit
   */
  auto& inputxEB = iInput.at("xEB");
  inputxEB.setShape(0, nHitsEB);
  auto dataxEB = inputxEB.allocate<float>();
  auto& vdataxEB = (*dataxEB)[0];

  auto& inputfEB = iInput.at("fEB");
  inputfEB.setShape(0, nHitsEB);
  auto datafEB = inputfEB.allocate<int64_t>();
  auto& vdatafEB = (*datafEB)[0];

  auto& inputGainEB = iInput.at("gainEB");
  inputGainEB.setShape(0, nHitsEB);
  auto dataGainEB = inputGainEB.allocate<int64_t>();
  auto& vdataGainEB = (*dataGainEB)[0];

  auto& inputGxEB = iInput.at("gxEB");
  inputGxEB.setShape(0, nPartEB_);
  auto dataGxEB = inputGxEB.allocate<float>();
  auto& vdataGxEB = (*dataGxEB)[0];

  auto& inputBatchEB = iInput.at("batchEB");
  inputBatchEB.setShape(0, nHitsEB);
  auto dataBatchEB = inputBatchEB.allocate<int64_t>();
  auto& vdataBatchEB = (*dataBatchEB)[0];

  auto& inputxEE = iInput.at("xEE");
  inputxEE.setShape(0, nHitsEE);
  auto dataxEE = inputxEE.allocate<float>();
  auto& vdataxEE = (*dataxEE)[0];

  auto& inputfEE = iInput.at("fEE");
  inputfEE.setShape(0, nHitsEE);
  auto datafEE = inputfEE.allocate<int64_t>();
  auto& vdatafEE = (*datafEE)[0];

  auto& inputGainEE = iInput.at("gainEE");
  inputGainEE.setShape(0, nHitsEE);
  auto dataGainEE = inputGainEE.allocate<int64_t>();
  auto& vdataGainEE = (*dataGainEE)[0];

  auto& inputGxEE = iInput.at("gxEE");
  inputGxEE.setShape(0, nPartEE_);
  auto dataGxEE = inputGxEE.allocate<float>();
  auto& vdataGxEE = (*dataGxEE)[0];

  auto& inputBatchEE = iInput.at("batchEE");
  inputBatchEE.setShape(0, nHitsEE);
  auto dataBatchEE = inputBatchEE.allocate<int64_t>();
  auto& vdataBatchEE = (*dataBatchEE)[0];

  auto& inputxES = iInput.at("xES");
  inputxES.setShape(0, nHitsES);
  auto dataxES = inputxES.allocate<float>();
  auto& vdataxES = (*dataxES)[0];

  auto& inputfES = iInput.at("fES");
  inputfES.setShape(0, nHitsES);
  auto datafES = inputfES.allocate<int64_t>();
  auto& vdatafES = (*datafES)[0];

  auto& inputBatchES = iInput.at("batchES");
  inputBatchES.setShape(0, nHitsES);
  auto dataBatchES = inputBatchES.allocate<int64_t>();
  auto& vdataBatchES = (*dataBatchES)[0];

  /*
   * Fill input tensors by iterating over particles...
   */
  int64_t partNumEB = 0, partNumEE = 0;
  std::shared_ptr<const CaloCellGeometry> geom;
  for (auto& part : *particles_) {
    const reco::SuperClusterRef& sc = part.superCluster();
    EB = isEB(part);

    if (skip(part))  //if neither EB or EE. Not sure if this even happens
      continue;

    std::vector<std::pair<DetId, float>> hitsAndFractions = sc->hitsAndFractions();
    EcalRecHitCollection::const_iterator hit;

    if (hitsAndFractions.empty()) //skip particles without RecHits
      continue;

    //iterate over ECAL hits...
    for (const auto& detitr : hitsAndFractions) {
      DetId id = detitr.first.rawId();
      if (EB) {
        geom = ecalEBGeom->getGeometry(id);
        hit = recHitsEB->find(detitr.first);
      } else {
        geom = ecalEEGeom->getGeometry(id);
        hit = recHitsEE->find(detitr.first);
      }

      //fill xECAL
      auto pos = geom->getPosition();
      if (EB){
        vdataxEB.push_back(rescale(pos.x(), XY_MIN, XY_RANGE));
        vdataxEB.push_back(rescale(pos.y(), XY_MIN, XY_RANGE));
        vdataxEB.push_back(rescale(pos.z(), Z_MIN, Z_RANGE));
        vdataxEB.push_back(rescale(hit->energy() * detitr.second, ECAL_MIN, ECAL_RANGE));     
        vdataxEB.push_back(rescale(ped_->find(detitr.first)->rms(1), NOISE_MIN, NOISE_RANGE));  
      } else {
        vdataxEE.push_back(rescale(pos.x(), XY_MIN, XY_RANGE));
        vdataxEE.push_back(rescale(pos.y(), XY_MIN, XY_RANGE));
        vdataxEE.push_back(rescale(pos.z(), Z_MIN, Z_RANGE));
        vdataxEE.push_back(rescale(hit->energy() * detitr.second, ECAL_MIN, ECAL_RANGE));     
        vdataxEE.push_back(rescale(ped_->find(detitr.first)->rms(1), NOISE_MIN, NOISE_RANGE));  
      }

      //fill fECAL
      int64_t flagVal = 0;
      if (hit->checkFlag(EcalRecHit::kGood))
        flagVal += 1;
      if (hit->checkFlag(EcalRecHit::kOutOfTime))
        flagVal += 2;
      if (hit->checkFlag(EcalRecHit::kPoorCalib))
        flagVal += 4;

      if(EB)
        vdatafEB.push_back(flagVal);
      else
        vdatafEE.push_back(flagVal);

      //fill gain
      int64_t gainVal = 0;
      if (hit->checkFlag(EcalRecHit::kHasSwitchToGain6))
        gainVal = 1;
      else if (hit->checkFlag(EcalRecHit::kHasSwitchToGain1))
        gainVal = 0;
      else
        gainVal = 2;

      if (EB)
        vdataGainEB.push_back(gainVal);
      else
        vdataGainEE.push_back(gainVal);

      //fill batch number
      if(EB)
        vdataBatchEB.push_back(partNumEB);
      else
        vdataBatchEE.push_back(partNumEE);
    }  //end iterate over ECAL hits

    //iterate over ES clusters...
    for (auto iES = sc->preshowerClustersBegin(); iES != sc->preshowerClustersEnd(); ++iES) {
      for (const auto ESitr : (*iES)->hitsAndFractions()) {  //iterate over ES hits
        hit = recHitsES->find(ESitr.first);
        geom = ecalESGeom->getGeometry(ESitr.first);
        auto& pos = geom->getPosition();

        //fill xES
        vdataxES.push_back(rescale(pos.x(), XY_MIN, XY_RANGE));
        vdataxES.push_back(rescale(pos.y(), XY_MIN, XY_RANGE));
        vdataxES.push_back(rescale(pos.z(), Z_MIN, Z_RANGE));
        vdataxES.push_back(rescale(hit->energy(), ES_MIN, ES_RANGE));

        //fill fES
        int64_t flagVal = 0;
        if (hit->checkFlag(EcalRecHit::kESGood))
          flagVal += 1;

        vdatafES.push_back(flagVal);

        //fill batchES
        vdataBatchES.push_back(partNumEE); //ES hits only exist for EE particles
      }  //end iterate over ES hits
    } //end iterate over ES clusters

    //fill gx
    if(EB){
      vdataGxEB.push_back(rescale(rho_, RHO_MIN, RHO_RANGE));
      vdataGxEB.push_back(rescale(part.hadronicOverEm(), HOE_MIN, HOE_RANGE));
    } else{
      vdataGxEE.push_back(rescale(rho_, RHO_MIN, RHO_RANGE));
      vdataGxEE.push_back(rescale(part.hadronicOverEm(), HOE_MIN, HOE_RANGE));
    }


    //increment particle number
    if(EB)
      ++partNumEB;
    else
      ++partNumEE;
  }  // end iterate over particles 

  /*
   * Convert input tensors to server data format
   */
  std::cout << "xEB: " << inputxEB.sizeShape() << std::endl;
  std::cout << "fEB: " << inputfEB.sizeShape() << std::endl;
  std::cout << "gainEB: " << inputGainEB.sizeShape() << std::endl;
  std::cout << "gxEB: " << inputGxEB.sizeShape() << std::endl;
  std::cout << "batchEB: " << inputBatchEB.sizeShape() << std::endl;
  std::cout << "xEE: " << inputxEE.sizeShape() << std::endl;
  std::cout << "fEE: " << inputfEE.sizeShape() << std::endl;
  std::cout << "gainEE: " << inputGainEE.sizeShape() << std::endl;
  std::cout << "gxEE: " << inputGxEE.sizeShape() << std::endl;
  std::cout << "batchEE: " << inputBatchEE.sizeShape() << std::endl;
  std::cout << "xES: " << inputxES.sizeShape() << std::endl;
  std::cout << "fES: " << inputfES.sizeShape() << std::endl;
  std::cout << "batchES: " << inputBatchES.sizeShape() << std::endl;
  inputxEB.toServer(dataxEB);
  inputfEB.toServer(datafEB);
  inputGainEB.toServer(dataGainEB);
  inputGxEB.toServer(dataGxEB);
  inputBatchEB.toServer(dataBatchEB);

  inputxEE.toServer(dataxEE);
  inputfEE.toServer(datafEE);
  inputGainEE.toServer(dataGainEE);
  inputGxEE.toServer(dataGxEE);
  inputBatchEE.toServer(dataBatchEE);

  inputxES.toServer(dataxES);
  inputfES.toServer(datafES);
  inputBatchES.toServer(dataBatchES);
}

template <typename T>
void DRNCorrectionProducerT<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) {

  particles_ = iEvent.getHandle(particleToken_);

  corrections_.clear();
  corrections_.reserve(nPart_);

  //if there are no particles, the fromServer() call will fail
  //but we can just put() an empty valueMap
  if(nPart_){
    const auto& muEB = iOutput.at("muEB").fromServer<float>();
    const auto& sigmaEB = iOutput.at("sigmaEB").fromServer<float>();
    const auto& muEE = iOutput.at("muEE").fromServer<float>();
    const auto& sigmaEE = iOutput.at("sigmaEE").fromServer<float>();

    for(unsigned q=0; q<5; ++q){
      std :: cout << q << ": " << sigmaEB[0][q] << std::endl;
    }

    unsigned iEB = 0, iEE = 0;
    float mu, sigma, Epred, sigmaPred, rawE;
    for (unsigned iPart = 0; iPart < nPart_; ++iPart) {
      const auto& part = particles_->at(iPart);
      if(!skip(part)) { 
        if(isEB(part)){
          mu = correction(muEB[0][0 + 6 * iEB]); 
          sigma = abs(sigmaEB[0][0 + 5 * iEB]); 
          ++iEB;
        } else{
          mu = correction(muEE[0][0 + 6 * iEE]); 
          sigma = abs(sigmaEE[0][0 + 5 * iEE]); 
          ++iEE;
        }

        std::cout<< "mu " << mu << std::endl;
        std::cout<< "sigma " << sigma << std::endl;

        rawE = particles_->at(iPart).superCluster()->rawEnergy();
        Epred = mu * rawE;
        sigmaPred = sigma * rawE;
        corrections_.emplace_back(std::pair<float, float>(Epred, sigmaPred)); 
      } else{
        corrections_.emplace_back(std::pair<float, float>(-1, -1));
      }
    }
  }

  //fill
  auto out = std::make_unique<edm::ValueMap<std::pair<float, float>>>();
  edm::ValueMap<std::pair<float, float>>::Filler filler(*out);
  filler.insert(particles_, corrections_.begin(), corrections_.end());
  filler.fill();

  iEvent.put(std::move(out));
}

template <typename T>
void DRNCorrectionProducerT<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("particleSource");
  desc.add<edm::InputTag>("rhoName");
  desc.add<edm::InputTag>("reducedEcalRecHitsEB");
  desc.add<edm::InputTag>("reducedEcalRecHitsEE");
  desc.add<edm::InputTag>("reducedEcalRecHitsES");
  descriptions.addWithDefaultLabel(desc);
}

using PatPhotonDRNCorrectionProducer = DRNCorrectionProducerT<pat::Photon>;
using GedPhotonDRNCorrectionProducer = DRNCorrectionProducerT<reco::Photon>;
using GsfElectronDRNCorrectionProducer = DRNCorrectionProducerT<reco::GsfElectron>;
using PatElectronDRNCorrectionProducer = DRNCorrectionProducerT<pat::Electron>;

//DEFINE_FWK_MODULE(DRNCorrectionProducerT);
DEFINE_FWK_MODULE(PatPhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(GedPhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(GsfElectronDRNCorrectionProducer);
DEFINE_FWK_MODULE(PatElectronDRNCorrectionProducer);
