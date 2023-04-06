#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "L1Trigger/L1THGCal/interface/AEutil.h"
#include "L1Trigger/L1THGCal/interface/TCSelector.h"

#include <map>
#include <iostream>

class MapMergeProducer : public edm::stream::EDProducer<> { 
  public:
    MapMergeProducer(const edm::ParameterSet&);
    ~MapMergeProducer() override {}

    void produce(edm::Event&, const edm::EventSetup&) override;
  private:
    std::vector<edm::EDGetTokenT<AEMap>> inputAETokens_;
    std::vector<edm::EDGetTokenT<ECONMap>> inputECONTokens_;
};

DEFINE_FWK_MODULE(MapMergeProducer);

MapMergeProducer::MapMergeProducer(const edm::ParameterSet& cfg){
  const auto inputs = cfg.getParameter<std::vector<std::string>>("inputs");
  for(const auto& input : inputs){
    std::cout << "A MapMergeProducer input is: " << input << std::endl;
    inputAETokens_.emplace_back(consumes<AEMap>(input));
    inputECONTokens_.emplace_back(consumes<ECONMap>(input));
  }
  produces<AEMap>();
  produces<ECONMap>();
}

void MapMergeProducer::produce(edm::Event& evt, const edm::EventSetup& es){
  auto AEout = std::make_unique<AEMap>();
  auto ECONout = std::make_unique<ECONMap>();

  for(unsigned int i=0; i<inputAETokens_.size(); ++i){
    edm::Handle<AEMap> AEHandle;
    edm::Handle<ECONMap> ECONHandle;
    
    evt.getByToken(inputAETokens_.at(i), AEHandle);
    evt.getByToken(inputECONTokens_.at(i), ECONHandle);

    std::cout << "Input " << i << " has " << AEHandle->size() << " entries" << std::endl;

    for(const auto& item : *(AEHandle.product())){
      (*AEout)[item.first] = item.second;
    }

    for(const auto& item : *(ECONHandle.product())){
      (*ECONout)[item.first] = item.second;
    }
  }
  std::cout << "Result has " << AEout->size() << " entries" << std::endl;
  
  evt.put(std::move(AEout));
  evt.put(std::move(ECONout));
}
