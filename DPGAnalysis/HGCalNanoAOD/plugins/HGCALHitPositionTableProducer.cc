#include "PhysicsTools/NanoAOD/interface/HitPositionTableProducer.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"

#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"

class HGCALHitPositionTableProducer : public HitPositionTableProducer<edm::View<CaloRecHit>> {
public:
  HGCALHitPositionTableProducer(edm::ParameterSet const& params)
      : HitPositionTableProducer(params) {}
        //caloGeomToken_(esConsumes<CaloGeometry, CaloGeometryRecord>()) {

  ~HGCALHitPositionTableProducer() override {}

  GlobalPoint positionFromHit(const CaloRecHit& hit) { return positionFromDetId(hit.detid()); }

  void beginRun(const edm::Run&, const edm::EventSetup& iSetup) override {
    // TODO: convert to esConsumes
    iSetup.get<CaloGeometryRecord>().get(caloGeom_);
    rhtools_.setGeometry(*caloGeom_);
  }

  GlobalPoint positionFromDetId(DetId id) {
    DetId::Detector det = id.det();
    if (det == DetId::HGCalEE || det == DetId::HGCalHSi || det == DetId::HGCalHSc) {
      return rhtools_.getPosition(id);
    } else {
      throw cms::Exception("HGCALHitPositionTableProducer") << "Unsupported DetId type";
    }
  }

protected:
  //edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeomToken_;
  edm::ESHandle<CaloGeometry> caloGeom_;
  hgcal::RecHitTools rhtools_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
typedef HGCALHitPositionTableProducer HGCALRecHitPositionTableProducer;
DEFINE_FWK_MODULE(HGCALRecHitPositionTableProducer);

