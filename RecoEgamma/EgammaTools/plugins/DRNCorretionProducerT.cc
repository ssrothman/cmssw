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

  size_t nPart_, nValidPart_;

  edm::ESHandle<EcalPedestals> ped_;
  edm::ESHandle<CaloGeometry> pG_;
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
void DRNCorrectionProducerT<T>::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) {
  //get things from the event...
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
  nValidPart_ = nPart_;

  corrections_.clear();
  corrections_.reserve(nPart_);

  HoEs_.clear();
  HoEs_.reserve(nPart_);

  if (nPart_ == 0) {
    client_->setBatchSize(0);
    return;
  } else {
    client_->setBatchSize(1);
  }

  unsigned totalHitsECAL = 0, totalHitsES = 0;
  bool isEB = 0, isEE = 0;
  //kinda awkward here
  //have to loop through everything an extra time to figure out how many hits there are
  //Not every particle has RecHits; no idea why
  //We will apply correction factors of 1 to those with no RecHits
  unsigned nHitsECAL; 
  for (auto& part : *particles_) {
    const reco::SuperClusterRef& sc = part.superCluster();
    isEB = ((*sc->seed()).hitsAndFractions().at(0).first.subdetId() == EcalBarrel);
    isEE = ((*sc->seed()).hitsAndFractions().at(0).first.subdetId() == EcalEndcap);

    if (!isEB && !isEE)  //if neither EB or EE. Not sure if this even happens
      continue;

    nHitsECAL = sc->hitsAndFractions().size();
    if (!nHitsECAL){ //no hits in the ECAL
      --nValidPart_;
      continue;
    }

    totalHitsECAL += nHitsECAL;

    for (auto iES = sc->preshowerClustersBegin(); iES != sc->preshowerClustersEnd(); ++iES) {
      totalHitsES += (*iES)->hitsAndFractions().size();
    }
  }

  //allocate model imputs
  auto& inputxECAL = iInput.at("xECAL__0");
  inputxECAL.setShape(0, totalHitsECAL);
  auto dataxECAL = inputxECAL.allocate<float>();
  auto& vdataxECAL = (*dataxECAL)[0];

  auto& inputfECAL = iInput.at("fECAL__1");
  inputfECAL.setShape(0, totalHitsECAL);
  auto datafECAL = inputfECAL.allocate<int64_t>();
  auto& vdatafECAL = (*datafECAL)[0];

  auto& inputGain = iInput.at("gain__2");
  inputGain.setShape(0, totalHitsECAL);
  auto dataGain = inputGain.allocate<int64_t>();
  auto& vdataGain = (*dataGain)[0];

  auto& inputxES = iInput.at("xES__3");
  inputxES.setShape(0, totalHitsES);
  auto dataxES = inputxES.allocate<float>();
  auto& vdataxES = (*dataxES)[0];

  auto& inputfES = iInput.at("fES__4");
  inputfES.setShape(0, totalHitsES);
  auto datafES = inputfES.allocate<int64_t>();
  auto& vdatafES = (*datafES)[0];

  auto& inputGx = iInput.at("graph_x__5");
  inputGx.setShape(0, nValidPart_);
  auto dataGx = inputGx.allocate<float>();
  auto& vdataGx = (*dataGx)[0];

  auto& inputBatchECAL = iInput.at("xECAL_batch__6");
  inputBatchECAL.setShape(0, totalHitsECAL);
  auto dataBatchECAL = inputBatchECAL.allocate<int64_t>();
  auto& vdataBatchECAL = (*dataBatchECAL)[0];

  auto& inputBatchES = iInput.at("xES_batch__7");
  inputBatchES.setShape(0, totalHitsES);
  auto dataBatchES = inputBatchES.allocate<int64_t>();
  auto& vdataBatchES = (*dataBatchES)[0];

  //fill model inputs
  //TODO: scaling
  int64_t partNum = 0;
  std::shared_ptr<const CaloCellGeometry> geom;

  unsigned nES = 0, nECAL = 0;

  //iterate over particles...
  for (auto& part : *particles_) {
    const reco::SuperClusterRef& sc = part.superCluster();
    isEB = ((*sc->seed()).hitsAndFractions().at(0).first.subdetId() == EcalBarrel);
    isEE = ((*sc->seed()).hitsAndFractions().at(0).first.subdetId() == EcalEndcap);

    if (!isEB && !isEE)  //if neither EB or EE. Not sure if this even happens
      continue;

    std::vector<std::pair<DetId, float>> hitsAndFractions = sc->hitsAndFractions();
    EcalRecHitCollection::const_iterator hit;

    if (hitsAndFractions.empty())
      continue;

    //iterate over ECAL hits...
    for (const auto& detitr : hitsAndFractions) {
      ++nECAL;
      DetId id = detitr.first.rawId();
      if (isEB) {
        geom = ecalEBGeom->getGeometry(id);
        hit = recHitsEB->find(detitr.first);
      } else {
        geom = ecalEEGeom->getGeometry(id);
        hit = recHitsEE->find(detitr.first);
      }

      //fill xECAL
      auto pos = geom->getPosition();
      vdataxECAL.push_back(rescale(pos.x(), XY_MIN, XY_RANGE));
      vdataxECAL.push_back(rescale(pos.y(), XY_MIN, XY_RANGE));
      vdataxECAL.push_back(rescale(pos.z(), Z_MIN, Z_RANGE));
      vdataxECAL.push_back(rescale(hit->energy() * detitr.second, ECAL_MIN, ECAL_RANGE));     //energy time fraction
      vdataxECAL.push_back(rescale(ped_->find(detitr.first)->rms(1), NOISE_MIN, NOISE_RANGE));  //noise pedestal

      //fill fECAL
      int64_t flagVal = 0;
      if (hit->checkFlag(EcalRecHit::kGood))
        flagVal += 1;
      if (hit->checkFlag(EcalRecHit::kOutOfTime))
        flagVal += 2;
      if (hit->checkFlag(EcalRecHit::kPoorCalib))
        flagVal += 4;

      vdatafECAL.push_back(flagVal);

      //fill gain
      if (hit->checkFlag(EcalRecHit::kHasSwitchToGain6))
        vdataGain.push_back(1);
      else if (hit->checkFlag(EcalRecHit::kHasSwitchToGain1))
        vdataGain.push_back(0);
      else
        vdataGain.push_back(2);

      //fill batchECAL
      vdataBatchECAL.push_back(partNum);
    }  //end iterate over ECAL hits

    //iterate over ES clusters...
    for (auto iES = sc->preshowerClustersBegin(); iES != sc->preshowerClustersEnd(); ++iES) {
      for (const auto ESitr : (*iES)->hitsAndFractions()) {  //iterate over hits
        ++nES;
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
        vdataBatchES.push_back(partNum);
      }  //end iterate over hits
    }    //end iterate over ES clusters

    //fill gx
    vdataGx.push_back(rescale(rho_, RHO_MIN, RHO_RANGE));
    vdataGx.push_back(rescale(part.hadronicOverEm(), HOE_MIN, HOE_RANGE));

    //increment particle number
    ++partNum;
  }  // end iterate over particles 

  //convert to server format
  inputxECAL.toServer(dataxECAL);
  inputfECAL.toServer(datafECAL);
  inputGain.toServer(dataGain);
  inputxES.toServer(dataxES);
  inputfES.toServer(datafES);
  inputBatchES.toServer(dataBatchES);
  inputGx.toServer(dataGx);
  inputBatchECAL.toServer(dataBatchECAL);
}

template <typename T>
void DRNCorrectionProducerT<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) {

  particles_ = iEvent.getHandle(particleToken_);

  //if there are no particles, the fromServer() call will fail
  //but we can just put() an empty valueMap
  if(nPart_){
    const auto& cvout = iOutput.at("CentralValue__0").fromServer<float>();
    const auto& resout = iOutput.at("Resolution__1").fromServer<float>();

    unsigned i = 0;
    for (unsigned iPart = 0; iPart < nPart_; ++iPart) {
      const reco::SuperClusterRef& sc = particles_->at(iPart).superCluster();
      if(sc->hitsAndFractions().size()) { //there were hits in the ECAL
        float mu, sigma, Epred, sigmaPred, rawE;
        mu = correction(cvout[0][0 + 6 * i]); //obtain correction factor from server output
        sigma = abs(resout[0][0 + 5 * i]); //obtain resolution from server output
        rawE = particles_->at(i).superCluster()->rawEnergy();
        Epred = mu * rawE;
        sigmaPred = sigma * rawE;
        corrections_.emplace_back(std::pair<float, float>(Epred, sigmaPred)); 
        ++i;
      } else{//no RecHits -> no corrections
        std::cout << "skipped photon " << iPart << std::endl;
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
using PhotonDRNCorrectionProducer = DRNCorrectionProducerT<reco::Photon>;
using GsfElectronDRNCorrectionProducer = DRNCorrectionProducerT<reco::GsfElectron>;
using PatElectronDRNCorrectionProducer = DRNCorrectionProducerT<pat::Electron>;

//DEFINE_FWK_MODULE(DRNCorrectionProducerT);
DEFINE_FWK_MODULE(PatPhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(PhotonDRNCorrectionProducer);
DEFINE_FWK_MODULE(GsfElectronDRNCorrectionProducer);
DEFINE_FWK_MODULE(PatElectronDRNCorrectionProducer);
