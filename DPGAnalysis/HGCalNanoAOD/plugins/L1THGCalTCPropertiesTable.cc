#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/CaloTest/interface/HGCalTestNumbering.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
#include "DataFormats/L1THGCal/interface/HGCalMulticluster.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/OneToMany.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticleFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"
#include "DataFormats/ForwardDetId/interface/HGCalTriggerDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCEEDetId.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"

class L1HGCalTCPropertiesTableProducer : public edm::stream::EDProducer<> {
public:
    explicit L1HGCalTCPropertiesTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken trigger_cells_token_;
    std::string name_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};

L1HGCalTCPropertiesTableProducer::L1HGCalTCPropertiesTableProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<edm::View<l1t::HGCalTriggerCell>>(conf.getParameter<edm::InputTag>("triggerCells"))),
      name_(conf.getParameter<std::string>("name")),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
    produces<nanoaod::FlatTable>();
}

void L1HGCalTCPropertiesTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void L1HGCalTCPropertiesTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalTriggerCell>> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);

    //common
    std::vector<int> det, sdet, side, layer;
    std::vector<float> x, y, z, eta, phi;
    //silicon
    std::vector<int> waferu, waferv, wafertype, cellu, cellv;
    //scint
    std::vector<int> paneltype, granularity, ieta, iphi, sipmsize, istrigger;


    for(const auto& tc : *trigger_cells_h) {
        uint32_t idint = tc.detId();

        DetId id(idint);

        det.push_back(id.det());
        side.push_back(triggerTools_.zside(id));
        layer.push_back(triggerTools_.layerWithOffset(id));

        if (id.det() == DetId::HGCalTrigger) {
            HGCalTriggerDetId detid(idint);

            sdet.push_back(detid.subdet());
            waferu.push_back(detid.waferU());
            waferv.push_back(detid.waferV());
            wafertype.push_back(detid.type());
            cellu.push_back(detid.triggerCellU());
            cellv.push_back(detid.triggerCellV());

            paneltype.push_back(-1);
            granularity.push_back(-1);
            ieta.push_back(-1);
            iphi.push_back(-1);
            sipmsize.push_back(-1);
            istrigger.push_back(-1);
        } else if (id.det() == DetId::HGCalHSc) {
            HGCScintillatorDetId detid(idint);

            sdet.push_back(0);
            paneltype.push_back(detid.type());
            granularity.push_back(detid.granularity());
            ieta.push_back(detid.ieta());
            iphi.push_back(detid.iphi());
            sipmsize.push_back(detid.sipm());
            istrigger.push_back(detid.trigger());

            waferu.push_back(-1);
            waferv.push_back(-1);
            wafertype.push_back(-1);
            cellu.push_back(-1);
            cellv.push_back(-1);
        } else {
            throw cms::Exception("L1HGCalTCPropertiesTableProducer") 
                << "Unsupported DetId type: " << id.det() << " for trigger cell with ID: " << idint;
        }
 
        const auto& pos = triggerTools_.getTriggerGeometry()->getTriggerCellPosition(idint);
        x.push_back(pos.x());
        y.push_back(pos.y());
        z.push_back(pos.z());
        eta.push_back(pos.eta());
        phi.push_back(pos.phi());
    }

    // Create the FlatTable
    auto table = std::make_unique<nanoaod::FlatTable>(sdet.size(), name_.c_str(), false, true);
    table->addColumn<int>("det", det, "Detector ID");
    table->addColumn<int>("subdet", sdet, "Subdetector ID");
    table->addColumn<int>("side", side, "Z side of the trigger cell");
    table->addColumn<int>("layer", layer, "Layer of the trigger cell");
    table->addColumn<float>("x", x, "X position of the trigger cell");
    table->addColumn<float>("y", y, "Y position of the trigger cell");
    table->addColumn<float>("z", z, "Z position of the trigger cell");
    table->addColumn<float>("eta", eta, "Eta of the trigger cell");
    table->addColumn<float>("phi", phi, "Phi of the trigger cell");
    table->addColumn<int>("waferu", waferu, "Wafer U coordinate");
    table->addColumn<int>("waferv", waferv, "Wafer V coordinate");
    table->addColumn<int>("wafertype", wafertype, "Wafer type");
    table->addColumn<int>("cellu", cellu, "Cell U coordinate");
    table->addColumn<int>("cellv", cellv, "Cell V coordinate");
    table->addColumn<int>("paneltype", paneltype, "Panel type");
    table->addColumn<int>("granularity", granularity, "Granularity of the panel");
    table->addColumn<int>("ieta", ieta, "iEta of the scintillator cell");
    table->addColumn<int>("iphi", iphi, "iPhi of the scintillator cell");
    table->addColumn<int>("sipmsize", sipmsize, "SiPM size in the scintillator cell");
    table->addColumn<int>("trigger", istrigger, "Is this a trigger cell?");
    evt.put(std::move(table));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1HGCalTCPropertiesTableProducer);
