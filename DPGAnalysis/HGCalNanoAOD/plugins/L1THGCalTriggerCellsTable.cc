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
#include "DataFormats/L1THGCal/interface/HGCalTriggerCellTruth.h"

class L1THGCalTriggerCellsTableProducer : public edm::stream::EDProducer<> {
public:
    explicit L1THGCalTriggerCellsTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken tc_truth_token_;


    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};


L1THGCalTriggerCellsTableProducer::L1THGCalTriggerCellsTableProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<edm::View<l1t::HGCalTriggerCell>>(conf.getParameter<edm::InputTag>("triggerCells"))),
      tc_truth_token_(consumes<edm::ValueMap<l1t::HGCalTriggerCellTruth>>(conf.getParameter<edm::InputTag>("tcTruth"))),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
    produces<nanoaod::FlatTable>("TCs");
    produces<nanoaod::FlatTable>("TCsToSimClusters");
}

void L1THGCalTriggerCellsTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void L1THGCalTriggerCellsTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<edm::View<l1t::HGCalTriggerCell>> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);

    // Get the trigger cell truth map
    edm::Handle<edm::ValueMap<l1t::HGCalTriggerCellTruth>> tc_truth_h;
    evt.getByToken(tc_truth_token_, tc_truth_h);
    const auto& tc_truth = *tc_truth_h;

    std::vector<int> ids, subdet, side, layer;
    std::vector<int> waferu, waferv, wafertype, cellu, cellv;
    std::vector<int> tcdata, uncompressedCharge, compressedCharge;
    std::vector<float> mipPt, pt, energy, simenergy;
    std::vector<float> eta, phi, x, y, z;
    std::vector<int> nSimCluster;
    std::vector<float> totSimClusterFrac;

    std::vector<int> simclusterIds;
    std::vector<float> simclusterFracs;

    for(size_t i = 0; i < trigger_cells_h->size(); ++i){
        const auto& tc = trigger_cells_h->at(i);
        const auto& tcref = trigger_cells_h->refAt(i);

        l1t::HGCalTriggerCellTruth tc_truth_info = tc_truth[tcref];

        uint32_t idint = tc.detId();

        DetId id(idint);
        if (id.det() == DetId::HGCalHSc){
            continue;
        }

        ids.push_back(idint);
        side.push_back(triggerTools_.zside(id));
        layer.push_back(triggerTools_.layerWithOffset(id));

        if (id.det() == DetId::HGCalTrigger){
            HGCalTriggerDetId detid(id);

            subdet.push_back(detid.subdet());
            waferu.push_back(detid.waferU());
            waferv.push_back(detid.waferV());
            wafertype.push_back(detid.type());
            cellu.push_back(detid.triggerCellU());
            cellv.push_back(detid.triggerCellV());
        } else {
            printf("Unsupported DetId type: %d\n", id.det());
            throw cms::Exception("L1THGCalTriggerCellsTableProducer") << "Unsupported DetId type";
        }

        const auto& pos = triggerTools_.getTriggerGeometry()->getTriggerCellPosition(idint);
        x.push_back(pos.x());
        y.push_back(pos.y());
        z.push_back(pos.z());
        eta.push_back(pos.eta());
        phi.push_back(pos.phi());

        tcdata.push_back(tc.hwPt());
        uncompressedCharge.push_back(tc.uncompressedCharge());
        compressedCharge.push_back(tc.compressedCharge());
        mipPt.push_back(tc.mipPt());
        pt.push_back(tc.pt());
        energy.push_back(tc.energy());

        simenergy.push_back(tc_truth_info.simEnergy());
        nSimCluster.push_back(tc_truth_info.nSimClusters());
        totSimClusterFrac.push_back(tc_truth_info.allocSimEnergyFraction());

        for (const auto& simcluster : tc_truth_info.simclusters()) {
            simclusterIds.push_back(simcluster.first);
            simclusterFracs.push_back(simcluster.second);
        }
    }

    // Create the FlatTable
    auto table = std::make_unique<nanoaod::FlatTable>(ids.size(), "L1HGCalHits", false);
    table->addColumn<int>("id", ids, "Raw ID of the trigger cell");
    table->addColumn<int>("subdet", subdet, "Subdetector ID");
    table->addColumn<int>("side", side, "Z side of the trigger cell");
    table->addColumn<int>("layer", layer, "Layer of the trigger cell");
    table->addColumn<int>("waferu", waferu, "Wafer U coordinate");
    table->addColumn<int>("waferv", waferv, "Wafer V coordinate");
    table->addColumn<int>("wafertype", wafertype, "Wafer type");
    table->addColumn<int>("cellu", cellu, "Cell U coordinate");
    table->addColumn<int>("cellv", cellv, "Cell V coordinate");
    table->addColumn<float>("x", x, "X position of the trigger cell");
    table->addColumn<float>("y", y, "Y position of the trigger cell");
    table->addColumn<float>("z", z, "Z position of the trigger cell");
    table->addColumn<float>("eta", eta, "Eta of the trigger cell");
    table->addColumn<float>("phi", phi, "Phi of the trigger cell");
    table->addColumn<int>("tcdata", tcdata, "Trigger cell data");
    table->addColumn<int>("uncompressedCharge", uncompressedCharge, "Uncompressed charge of the trigger cell");
    table->addColumn<int>("compressedCharge", compressedCharge, "Compressed charge of the trigger cell");
    table->addColumn<float>("mipPt", mipPt, "Mip Pt of the trigger cell");
    table->addColumn<float>("pt", pt, "Pt of the trigger cell");
    table->addColumn<float>("energy", energy, "Energy of the trigger cell");
    table->addColumn<float>("simenergy", simenergy, "Simulated energy of the trigger cell");
    table->addColumn<int>("nSimCluster", nSimCluster, "Number of SimClusters contributing to the trigger cell");
    table->addColumn<float>("totSimClusterFrac", totSimClusterFrac, "Total fraction of SimCluster energy contributing to the trigger cell");
    evt.put(std::move(table), "TCs");

    auto simclust_table = std::make_unique<nanoaod::FlatTable>(simclusterIds.size(), "L1HGCalHitsToSimclusters", false);
    simclust_table->addColumn<int>("idx", simclusterIds, "ID of the SimCluster contributing to the trigger cell");
    simclust_table->addColumn<float>("frac", simclusterFracs, "Fraction of the SimCluster energy contributing to the trigger cell");
    evt.put(std::move(simclust_table), "TCsToSimClusters");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1THGCalTriggerCellsTableProducer);
