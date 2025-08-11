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

class L1THGCalTriggerCellsTableProducer : public edm::stream::EDProducer<> {
public:
    explicit L1THGCalTriggerCellsTableProducer(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    //static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;
private:
    double calibrate(double energy, unsigned thickness, unsigned layer);

    edm::EDGetToken trigger_cells_token_;
    edm::EDGetToken simhits_ee_token_;
    edm::EDGetToken simhits_fh_token_;
    edm::EDGetToken simhits_bh_token_;
    edm::EDGetToken simclusters_token_;

    double keV2fC_;
    std::vector<double> fcPerMip_;
    std::vector<double> layerWeights_;
    std::vector<double> thicknessCorrections_;

    edm::ESGetToken<HGCalTriggerGeometryBase, CaloGeometryRecord> triggerGeomToken_;

    HGCalTriggerTools triggerTools_;
};


L1THGCalTriggerCellsTableProducer::L1THGCalTriggerCellsTableProducer(const edm::ParameterSet& conf)
    : trigger_cells_token_(consumes<l1t::HGCalTriggerCellBxCollection>(conf.getParameter<edm::InputTag>("triggerCells"))),
      simhits_ee_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsEE"))),
      simhits_fh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsFH"))),
      simhits_bh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("simHitsBH"))),
      simclusters_token_(consumes<std::vector<SimCluster>>(conf.getParameter<edm::InputTag>("simClusters"))),
      keV2fC_(conf.getParameter<double>("keV2fC")),
      fcPerMip_(conf.getParameter<std::vector<double>>("fcPerMip")),
      layerWeights_(conf.getParameter<std::vector<double>>("layerWeights")),
      thicknessCorrections_(conf.getParameter<std::vector<double>>("thicknessCorrections")),
      triggerGeomToken_(esConsumes<HGCalTriggerGeometryBase, CaloGeometryRecord, edm::Transition::BeginRun>())
{
    produces<nanoaod::FlatTable>("HGCalTriggerCellsTableProducer");
}

double L1THGCalTriggerCellsTableProducer::calibrate(double energy, unsigned thickness, unsigned layer) {
    double fcPerMip = fcPerMip_[thickness];
    double thicknessCorrection = thicknessCorrections_[thickness];
    double layerWeight = layerWeights_[layer];
    double TeV2GeV = 1000.0;
  return energy * keV2fC_ / fcPerMip * layerWeight * TeV2GeV / thicknessCorrection;
}

void L1THGCalTriggerCellsTableProducer::beginRun(const edm::Run&, const edm::EventSetup& es) {
    const auto& triggerGeometry = es.getHandle(triggerGeomToken_);
    triggerTools_.setGeometry(triggerGeometry.product());
}

void L1THGCalTriggerCellsTableProducer::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the trigger cells
    edm::Handle<l1t::HGCalTriggerCellBxCollection> trigger_cells_h;
    evt.getByToken(trigger_cells_token_, trigger_cells_h);
    const auto& trigger_cells = *trigger_cells_h;

    // Get the simhits
    edm::Handle<edm::PCaloHitContainer> simhits_ee_h;
    evt.getByToken(simhits_ee_token_, simhits_ee_h);
    const auto& simhits_ee = *simhits_ee_h;

    edm::Handle<edm::PCaloHitContainer> simhits_fh_h;
    evt.getByToken(simhits_fh_token_, simhits_fh_h);
    const auto& simhits_fh = *simhits_fh_h;

    edm::Handle<edm::PCaloHitContainer> simhits_bh_h;
    evt.getByToken(simhits_bh_token_, simhits_bh_h);
    const auto& simhits_bh = *simhits_bh_h;

    // Get the simclusters
    edm::Handle<std::vector<SimCluster>> simclusters_h;
    evt.getByToken(simclusters_token_, simclusters_h);

    std::unordered_map<uint32_t, l1t::HGCalTriggerCell> hits_map;
    std::unordered_map<uint32_t, std::vector<PCaloHit>> simhits_map;
    std::set<uint32_t> unique_ids;

    std::unordered_map<uint32_t, std::vector<std::pair<int, float>>> simclusters_map;

    // Fill the hits map
    for (const auto& tc : trigger_cells) {
        hits_map[tc.detId()] = tc;
        unique_ids.insert(tc.detId());
    }

    // Fill the simhits map
    for (const auto& hit : simhits_ee) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
        unique_ids.insert(tcid);
    }
    for (const auto& hit : simhits_fh) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
        unique_ids.insert(tcid);
    }
    for (const auto& hit : simhits_bh) {
        uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.id());
        simhits_map[tcid].push_back(hit);
        unique_ids.insert(tcid);
    }

    // Fill the simclusters map
    for (uint32_t iSimCluster = 0; iSimCluster < simclusters_h->size(); ++iSimCluster) {
        const auto& simcluster = (*simclusters_h)[iSimCluster];
        for (const auto& hit : simcluster.hits_and_fractions()){
            uint32_t tcid = triggerTools_.getTriggerGeometry()->getTriggerCellFromCell(hit.first);
            if (tcid != 0) { // Only consider valid trigger cells
                simclusters_map[hit.first].emplace_back(iSimCluster, hit.second);
                unique_ids.insert(tcid);
            }
        }
    }

    std::vector<int> ids, subdet, side, layer;
    std::vector<int> waferu, waferv, wafertype, cellu, cellv;
    std::vector<int> tcdata, uncompressedCharge, compressedCharge;
    std::vector<float> mipPt, pt, energy, simenergy;
    std::vector<float> eta, phi, x, y, z;
    std::vector<int> nSimCluster;
    std::vector<float> totSimClusterFrac;

    for(const auto& idint : unique_ids){
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

        const auto& hit = hits_map.find(idint); 
        if (hit == hits_map.end()) {
            tcdata.push_back(0);
            uncompressedCharge.push_back(0);
            compressedCharge.push_back(0);
            mipPt.push_back(0);
            pt.push_back(0);
            energy.push_back(0);
        } else {
            const auto& tc = hit->second;
            tcdata.push_back(tc.hwPt());
            uncompressedCharge.push_back(tc.uncompressedCharge());
            compressedCharge.push_back(tc.compressedCharge());
            mipPt.push_back(tc.mipPt());
            pt.push_back(tc.pt());
            energy.push_back(tc.energy());
        }

        const auto& simhitvec = simhits_map.find(idint);
        if (simhitvec == simhits_map.end() || simhitvec->second.empty()) {
            simenergy.push_back(0);
        } else {
            float E=0;
            unsigned thickness = triggerTools_.thicknessIndex(idint);
            unsigned layer = triggerTools_.layerWithOffset(idint);

            int nSimClus = 0;
            float totFracE = 0;

            for(const auto& simhit : simhitvec->second){
                E += calibrate(simhit.energy(), thickness, layer);
                const auto& simcluss = simclusters_map.find(simhit.id());
                if (simcluss != simclusters_map.end()) {
                    for (const auto& simclus : simcluss->second){
                        ++nSimClus;
                        totFracE += simclus.second * calibrate(simhit.energy(), thickness, layer);
                    }
                }
            }
            simenergy.push_back(E);
            nSimCluster.push_back(nSimClus);
            if (E == 0) {
                totFracE = 0; // Avoid division by zero
            } else {
                totFracE /= E; // Normalize the total fraction
            }
            totSimClusterFrac.push_back(totFracE);
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
    evt.put(std::move(table), "HGCalTriggerCellsTableProducer");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1THGCalTriggerCellsTableProducer);
