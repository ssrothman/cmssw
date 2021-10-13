#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"
#include "HeterogeneousCore/SonicTriton/interface/TritonData.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

#include "RecoEcal/EgammaClusterAlgos/interface/SCEnergyCorrectorSemiParm_DRN.h"

#include <sstream>
#include <string>
#include <vector>
#include <random>

/*
 * SCEnergyCorrectorProducer_DRN
 *
 * Simple producer to generate a set of corrected superclusters with the DRN regression
 * Based on RecoEcal/EgammaClusterProducers/SCEnergyCorrectorProducer by S. Harper (RAL/CERN)
 *
 * Author: Simon Rothman (UMN, MIT)
 *
 */

static float sigmoid(float x){
    return 1.0f/(1.0f + exp(-x));
}
static float ln2 = log(2);

static float logcorrection(float x){
    return ln2 * 2 * (sigmoid(x) - 0.5);
}

static float correction(float x){
    return exp(-logcorrection(x));
}

class SCEnergyCorrectorProducer_DRN : public TritonEDProducer<> {
public:
  explicit SCEnergyCorrectorProducer_DRN(const edm::ParameterSet& iConfig);

  void beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& iSetup) override;

  void acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& input) override;
  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput) override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  SCEnergyCorrectorSemiParm_DRN energyCorrector_;
  edm::EDGetTokenT<reco::SuperClusterCollection> inputSCToken_;
};

SCEnergyCorrectorProducer_DRN::SCEnergyCorrectorProducer_DRN(const edm::ParameterSet& iConfig)
    : TritonEDProducer<>(iConfig, "SCEnergyCorrectorProducer_DRN"),
      energyCorrector_(iConfig.getParameterSet("correctorCfg"), consumesCollector()),
      inputSCToken_(consumes<reco::SuperClusterCollection>(iConfig.getParameter<edm::InputTag>("inputSCs")))
{
  produces<reco::SuperClusterCollection>();
}

void SCEnergyCorrectorProducer_DRN::beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& iSetup) {
  energyCorrector_.setEventSetup(iSetup);
}

void SCEnergyCorrectorProducer_DRN::acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput){
    auto inputSCs = iEvent.get(inputSCToken_);

    if(inputSCs.size()==0){
        client_->setBatchSize(0);
        return;
    } else{
        client_->setBatchSize(1);
    }

    energyCorrector_.setEvent(iEvent);
    energyCorrector_.make_input(iEvent, iInput, inputSCs);
}

void SCEnergyCorrectorProducer_DRN::produce(edm::Event& iEvent, const edm::EventSetup& iSetup, Output const& iOutput){
    auto inputSCs = iEvent.get(inputSCToken_);
    if (inputSCs.size()==0)
        return;

    const auto& serverout = energyCorrector_.get_output(iOutput);

    if(inputSCs.size() ==0){
        return;
    }

    auto corrSCs = std::make_unique<reco::SuperClusterCollection>();
    unsigned i=0;
    for (const auto& inputSC : inputSCs) {
        float corrEn = correction(serverout[0][0+6*i]) * inputSC.rawEnergy(); 
        corrSCs->push_back(inputSC);
        corrSCs->back().setEnergy(corrEn);
        corrSCs->back().setCorrectedEnergy(corrEn);
        //corrSCs->back().setCorrectedEnergyUncertainty(-1.0);
        ++i;
    }

    auto scHandle = iEvent.put(std::move(corrSCs));
}

void SCEnergyCorrectorProducer_DRN::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::ParameterSetDescription>("correctorCfg", SCEnergyCorrectorSemiParm_DRN::makePSetDescription());
  TritonClient::fillPSetDescription(desc);
  desc.add<edm::InputTag>("inputSCs", edm::InputTag("particleFlowSuperClusterECAL"));
  descriptions.add("scEnergyCorrectorProducer_DRN", desc);
}

DEFINE_FWK_MODULE(SCEnergyCorrectorProducer_DRN);
