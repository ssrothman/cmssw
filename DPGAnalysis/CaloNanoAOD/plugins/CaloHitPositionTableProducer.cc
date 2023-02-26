#include "PhysicsTools/NanoAOD/interface/HitPositionTableProducer.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCScintillatorDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCSiliconDetId.h"

#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"


template <typename T>
class CaloHitPositionTableProducer : public HitPositionTableProducer<edm::View<T>> {
  public:
    CaloHitPositionTableProducer(edm::ParameterSet const& params)
        : HitPositionTableProducer<edm::View<T>>(params),
          caloGeoToken_(edm::stream::EDProducer<>::esConsumes<edm::Transition::BeginRun>()) { }

    ~CaloHitPositionTableProducer() override {}

    GlobalPoint positionFromHit(const CaloRecHit& hit) { 
      DetId detId = hit.detid();
      return positionFromDetId(detId); 
    }

    GlobalPoint positionFromHit(const PCaloHit& hit) { 
      DetId detId = hit.id();
      return positionFromDetId(detId); 
    }

    float radiusFromHit(const CaloRecHit& hit) { 
      return radiusFromDetId(hit.detid()); 
    }

    float radiusFromHit(const PCaloHit& hit) { 
      return radiusFromDetId(hit.id()); 
    }

    void beginRun(const edm::Run&, const edm::EventSetup& iSetup) override {
      caloGeom_ = &iSetup.getData(caloGeoToken_);
      rhtools_.setGeometry(*caloGeom_);

      ecalEBGeom = caloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
      ecalEEGeom = caloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
    }

    GlobalPoint positionFromDetId(DetId id) {
      DetId::Detector det = id.det();
      if (det == DetId::HGCalEE || det == DetId::HGCalHSi || det == DetId::HGCalHSc) {
        return rhtools_.getPosition(id);
      } else if(det == DetId::Ecal){
        if(id.subdetId() == EcalBarrel){
          return ecalEBGeom->getGeometry(id)->getPosition();
        } else if(id.subdetId() == EcalEndcap){
          return ecalEEGeom->getGeometry(id)->getPosition();
        } else {
          throw cms::Exception("CaloHitPositionTableProducer") << "Unsupported ECAL subdet " << id.subdetId();
        }
      } else if(det == DetId::Hcal){
        GlobalPoint position;
        HcalSubdetector esd = (HcalSubdetector)id.subdetId();
        const auto* gTmp = caloGeom_->getSubdetectorGeometry(DetId::Hcal, esd);
        return gTmp->getGeometry(id)->getPosition();
      }else {
        std::ostringstream message;
        message << "Unsupported DetId: ";
        if (det == DetId::Tracker){
          message << "Tracker";
        } else if (det == DetId::Muon){
          message << "Muon system";
        } else if (det == DetId::Calo){
          message << "Generic Calo???";
        } else if (det == DetId::Forward){
          message << "Forward??";
        } else if (det == DetId::VeryForward){
          message << "Very Forward??";
        } else {
          message << "Unknown value " << det;
        }
        throw cms::Exception("CaloHitPositionTableProducer") << "Unsupported DetId type";
      }
    }

    float radiusFromDetId(DetId id) {
      DetId::Detector det = id.det();
      if (det == DetId::HGCalEE || det == DetId::HGCalHSi || det == DetId::HGCalHSc) {
        if (rhtools_.isSilicon(id)) {
          return rhtools_.getRadiusToSide(id);
        } else if (rhtools_.isScintillator(id)) {
            //return r * sin(dphi) / 2.;
         // auto detadphi = rhtools_.getScintDEtaDPhi(id);
         // float dphi = detadphi.second;  //same in both
          float dphi = 0.0211809f; //DEBUG FIXME; the geometry access segfaults for some hits probably close to side
          auto pos = rhtools_.getPosition(id);
          float r = pos.transverse();
          return r * sin(dphi) / 2.;  //this is anyway approximate
        } else {
          return 0.;
        }

      } else {
        return 0;  //no except here
      }
    }

  protected:
    edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeoToken_;
    hgcal::RecHitTools rhtools_;
    const CaloGeometry* caloGeom_;

    const CaloSubdetectorGeometry* ecalEBGeom;
    const CaloSubdetectorGeometry* ecalEEGeom;

};

#include "FWCore/Framework/interface/MakerMacros.h"
typedef CaloHitPositionTableProducer<CaloRecHit> CaloRecHitPositionTableProducer;
typedef CaloHitPositionTableProducer<PCaloHit> PCaloHitPositionTableProducer;

DEFINE_FWK_MODULE(CaloRecHitPositionTableProducer);
DEFINE_FWK_MODULE(PCaloHitPositionTableProducer);
