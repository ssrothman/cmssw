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

    void beginRun(const edm::Run&, const edm::EventSetup& iSetup) override {
      caloGeom_ = &iSetup.getData(caloGeoToken_);

      ecalEBGeom = caloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
      ecalEEGeom = caloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
    }

    GlobalPoint positionFromDetId(DetId id) {
      printf("top of positionFromDetId()\n");
      DetId::Detector det = id.det();
      printf("got det\n");
      if (det == DetId::HGCalEE || det == DetId::HGCalHSi || det == DetId::HGCalHSc) {
        printf("HGCAL\n");
        throw cms::Exception("CaloHitPositionTableProducer") << "HGCAL not supported";
      } else if(det == DetId::Ecal){
        printf("ECAL\n");
        if(id.subdetId() == EcalBarrel){
          return ecalEBGeom->getGeometry(id)->getPosition();
        } else if(id.subdetId() == EcalEndcap){
          return ecalEEGeom->getGeometry(id)->getPosition();
        } else {
          throw cms::Exception("CaloHitPositionTableProducer") << "Unsupported ECAL subdet " << id.subdetId();
        }
      } else if(det == DetId::Hcal){
        printf("HCAL\n");
        HcalSubdetector esd = (HcalSubdetector)id.subdetId();
        printf("esd = %d \n", esd);
        const CaloSubdetectorGeometry* gTmp = caloGeom_->getSubdetectorGeometry(DetId::Hcal, esd);
        printf("gTMP\n");
        auto geo = gTmp->getGeometry(id);
        printf("geo\n");
        GlobalPoint pos = geo->getPosition();
        printf("pos\n");
        return pos;
      }else {
        printf("uh oh\n");
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

  protected:
    edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeoToken_;
    const CaloGeometry* caloGeom_;

    const CaloSubdetectorGeometry* ecalEBGeom;
    const CaloSubdetectorGeometry* ecalEEGeom;

};

#include "FWCore/Framework/interface/MakerMacros.h"
typedef CaloHitPositionTableProducer<CaloRecHit> CaloRecHitPositionTableProducer;
typedef CaloHitPositionTableProducer<PCaloHit> PCaloHitPositionTableProducer;

DEFINE_FWK_MODULE(CaloRecHitPositionTableProducer);
DEFINE_FWK_MODULE(PCaloHitPositionTableProducer);
