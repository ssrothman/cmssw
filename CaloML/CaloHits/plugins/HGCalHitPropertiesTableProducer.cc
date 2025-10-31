#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "DataFormats/Common/interface/View.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include <vector>
#include <iostream>

template <typename T>
class HGCalHitPropertiesTableProducer : public edm::stream::EDProducer<> {
public:
  HGCalHitPropertiesTableProducer(edm::ParameterSet const& params)
      : name_(params.getParameter<std::string>("name")),
        doc_(params.getParameter<std::string>("doc")),
        cut_(params.getParameter<std::string>("cut"), true) {
    produces<nanoaod::FlatTable>();

    const std::vector<edm::InputTag> srctags = params.getParameter<std::vector<edm::InputTag>>("src");
    for (const auto& tag : srctags) {
        srcs_.emplace_back(consumes<T>(tag));
    }
  }

  ~HGCalHitPropertiesTableProducer() override {}

  uint32_t detIdFromHit(const PCaloHit& hit) { 
    return hit.id();
  }

  uint32_t detIdFromHit(const CaloRecHit& hit) { 
    return hit.detid();
  }

  uint32_t detIdFromHit(const reco::PFRecHit& hit) { 
    return hit.detId();
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override {
    edm::Handle<T> objs;

    std::vector<int> subdet, layer, hwType, zside;

    for (const auto& src : srcs_){
        iEvent.getByToken(src, objs);

        for (const auto& obj : *objs) {
            if (cut_(obj)) {
                DetId detid(detIdFromHit(obj));
                DetId::Detector det = detid.det();

                if (det == DetId::HGCalEE || det == DetId::HGCalHSi){
                    const HGCSiliconDetId hgcid(detid);

                    subdet.push_back(hgcid.subdet());
                    layer.push_back(hgcid.layer());
                    hwType.push_back(hgcid.type());
                    zside.push_back(hgcid.zside());

                } else if (det == DetId::HGCalHSc){
                    const HGCScintillatorDetId hgcid(detid);

                    int sipmtype = hgcid.sipm(); //two values
                    int granularity = hgcid.granularity(); //two values
                    int tiletype = hgcid.type(); //four values
                    
                    int type = sipmtype + 2*granularity + 4*tiletype;

                    subdet.push_back(hgcid.subdet());
                    layer.push_back(hgcid.layer());
                    hwType.push_back(type);
                    zside.push_back(hgcid.zside());
                } else {
                    throw cms::Exception("HGCalHitPropertiesTableProducer") << "Unsupported DetId type" << detid.det();
                }
            }
        }
    }

    auto tab = std::make_unique<nanoaod::FlatTable>(subdet.size(), name_, false, false);
    tab->addColumn<int>("subdet", subdet, "HGCal subdetector");
    tab->addColumn<int>("layer", layer, "HGCal layer number");
    tab->addColumn<int>("hwType", hwType, "HGCal hardware type");
    tab->addColumn<int>("zside", zside, "HGCal z-side (+1 or -1)");
    
    iEvent.put(std::move(tab));
  }

protected:
  const std::string name_, doc_;
  std::vector<edm::EDGetTokenT<T>> srcs_;
  const StringCutObjectSelector<typename T::value_type> cut_;
};

typedef HGCalHitPropertiesTableProducer<edm::View<PCaloHit>> HGCalSimHitPropertiesTableProducer;
typedef HGCalHitPropertiesTableProducer<edm::View<CaloRecHit>> HGCalCaloRecHitPropertiesTableProducer;
typedef HGCalHitPropertiesTableProducer<edm::View<reco::PFRecHit>> HGCalPFRecHitPropertiesTableProducer;

DEFINE_FWK_MODULE(HGCalSimHitPropertiesTableProducer);
DEFINE_FWK_MODULE(HGCalCaloRecHitPropertiesTableProducer);
DEFINE_FWK_MODULE(HGCalPFRecHitPropertiesTableProducer);