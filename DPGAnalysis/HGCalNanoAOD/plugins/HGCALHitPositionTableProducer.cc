#include "PhysicsTools/NanoAOD/interface/HitPositionTableProducer.h"

#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"

template <typename T>
class HGCalHitPositionTableProducer : public HitPositionTableProducer<edm::View<T>> {
//class HGCalHitPositionTableProducer : public HitPositionTableProducer<edm::View<CaloRecHit>> {
public:
  HGCalHitPositionTableProducer(edm::ParameterSet const& params)
      : HitPositionTableProducer<edm::View<T>>(params), 
        caloGeoToken_(edm::stream::EDProducer<>::esConsumes<edm::Transition::BeginRun>()) {}

  ~HGCalHitPositionTableProducer() override {}

  GlobalPoint positionFromHit(const CaloRecHit& hit) { 
    DetId detId = hit.detid();
    return positionFromDetId(detId); 
  }

  GlobalPoint positionFromHit(const PCaloHit& hit) { 
    DetId detId = hit.id();
    return positionFromDetId(detId); 
  }

  void beginRun(const edm::Run&, const edm::EventSetup& iSetup) override {
    auto& geom = iSetup.getData(caloGeoToken_);
    rhtools_.setGeometry(geom);

    ecalEBGeom = geom.getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
    ecalEEGeom = geom.getSubdetectorGeometry(DetId::Ecal, EcalEndcap);

    hcalEBGeom = geom.getSubdetectorGeometry(DetId::Hcal, HcalBarrel);
    hcalEEGeom = geom.getSubdetectorGeometry(DetId::Hcal, HcalEndcap);
  }

  GlobalPoint positionFromDetId(DetId id) {
    DetId::Detector det = id.det();
    if (det == DetId::HGCalEE || det == DetId::HGCalHSi || det == DetId::HGCalHSc) {
      return rhtools_.getPosition(id);
    } else if(id.subdetId() == EcalBarrel){
      return ecalEBGeom->getGeometry(id)->getPosition();
    } else if(id.subdetId() == EcalEndcap){
      return ecalEEGeom->getGeometry(id)->getPosition();
    } else if(id.subdetId() == HcalBarrel){
      return hcalEBGeom->getGeometry(id)->getPosition();
    } else if(id.subdetId() == HcalEndcap){
      return hcalEEGeom->getGeometry(id)->getPosition();
    } else {
      throw cms::Exception("HGCalHitPositionTableProducer") << "Unsupported detector";
    }
  }

protected:
  edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeoToken_;
  hgcal::RecHitTools rhtools_;
  const CaloSubdetectorGeometry* ecalEBGeom;
  const CaloSubdetectorGeometry* ecalEEGeom;
  const CaloSubdetectorGeometry* hcalEBGeom;
  const CaloSubdetectorGeometry* hcalEEGeom;

};

#include "FWCore/Framework/interface/MakerMacros.h"
//typedef HGCalHitPositionTableProducer HGCalRecHitPositionTableProducer;
typedef HGCalHitPositionTableProducer<CaloRecHit> HGCalRecHitPositionTableProducer;
typedef HGCalHitPositionTableProducer<PCaloHit> HGCalSimHitPositionTableProducer;
DEFINE_FWK_MODULE(HGCalRecHitPositionTableProducer);
DEFINE_FWK_MODULE(HGCalSimHitPositionTableProducer);
