#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "DataFormats/Common/interface/View.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "RecoHGCal/GraphReco/interface/HGCalTrackPropagator.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include <vector>

class TrackPositionAtHGCALTableProducer : public edm::stream::EDProducer<> {
public:
  TrackPositionAtHGCALTableProducer(edm::ParameterSet const& params)
      : name_(params.getParameter<std::string>("name")),
        src_(consumes<reco::TrackCollection>(params.getParameter<edm::InputTag>("src"))),
        cut_(params.getParameter<std::string>("cut"), true),
        bFieldToken_(esConsumes<MagneticField, IdealMagneticFieldRecord>()),
        hgcEEToken_(esConsumes<edm::Transition::BeginRun>(edm::ESInputTag{"", "HGCalEESensitive"})),
        propagatorToken_(esConsumes(edm::ESInputTag("", "PropagatorWithMaterial"))) {
    produces<nanoaod::FlatTable>();
  }

  ~TrackPositionAtHGCALTableProducer() override {}

  void beginRun(const edm::Run&, const edm::EventSetup& iSetup) override {
    const MagneticField* bField = &iSetup.getData(bFieldToken_);
    const Propagator* prop = &iSetup.getData(propagatorToken_);
    const HGCalDDDConstants* ddd = &iSetup.getData(hgcEEToken_);

    trackprop_.initialize(prop, bField, ddd);
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override {
    edm::Handle<reco::TrackCollection> objs;
    iEvent.getByToken(src_, objs);

    std::vector<float> xvals;
    std::vector<float> yvals;
    std::vector<float> zvals;
    std::vector<float> phis;
    std::vector<float> etas;
    for (const auto& obj : *objs) {
      if (cut_(obj)) {
        auto prop = trackprop_.propagateObject(obj);
        xvals.emplace_back(prop.pos.x());
        yvals.emplace_back(prop.pos.y());
        zvals.emplace_back(prop.pos.z());
        etas.emplace_back(prop.pos.eta());
        phis.emplace_back(prop.pos.phi());
      }
    }

    auto tab = std::make_unique<nanoaod::FlatTable>(xvals.size(), name_, false, true);
    tab->addColumn<float>("HGCFront_x", xvals, "x position at front face of HGCAL");
    tab->addColumn<float>("HGCFront_y", yvals, "y position at front face of HGCAL");
    tab->addColumn<float>("HGCFront_z", zvals, "z position at front face of HGCAL");
    tab->addColumn<float>("HGCFront_eta", etas, "eta position at front face of HGCAL");
    tab->addColumn<float>("HGCFront_phi", phis, "phi position at front face of HGCAL");

    iEvent.put(std::move(tab));
  }

protected:
  const std::string name_;
  const edm::EDGetTokenT<reco::TrackCollection> src_;
  const StringCutObjectSelector<reco::Track> cut_;
  HGCalTrackPropagator trackprop_;
  edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> bFieldToken_;
  edm::ESGetToken<HGCalDDDConstants, CaloGeometryRecord> hgcEEToken_;
  edm::ESGetToken<Propagator, TrackingComponentsRecord> propagatorToken_;
};

DEFINE_FWK_MODULE(TrackPositionAtHGCALTableProducer);
