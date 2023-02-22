#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"

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
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "L1Trigger/L1THGCalUtilities/interface/HGCalTriggerNtupleBase.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerTools.h"


class TCTableProducer : public edm::stream::EDProducer<> {
  public: 
    explicit TCTableProducer(const edm::ParameterSet&);
    ~TCTableProducer() override {}
    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    void produce(edm::Event&, const edm::EventSetup&) override;
  private:
    double calibrate(double, int, unsigned);
    void simhits(const edm::Event& e,
                 std::unordered_map<uint32_t, double>& simhits_ee,
                 std::unordered_map<uint32_t, double>& simhits_fh,
                 std::unordered_map<uint32_t, double>& simhits_bh);

    HGCalTriggerTools triggerTools_;

    edm::EDGetToken tc_token_;
    edm::EDGetToken multiclusters_token_;
    edm::EDGetToken simhits_ee_token_, simhits_fh_token_, simhits_bh_token_;

    double keV2fC_;
    std::vector<double> fcPerMip_;
    std::vector<double> layerWeights_;
    std::vector<double> thicknessCorrections_;

    edm::ESHandle<HGCalTriggerGeometryBase> geometry_;
};

TCTableProducer::TCTableProducer(const edm::ParameterSet& conf)
  : tc_token_(consumes<l1t::HGCalTriggerCellBxCollection>(conf.getParameter<edm::InputTag>("TriggerCells"))),
    multiclusters_token_(consumes<l1t::HGCalMulticlusterBxCollection>(conf.getParameter<edm::InputTag>("MultiClusters"))),
    simhits_ee_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("eeSimHits"))),
    simhits_fh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("fhSimHits"))),
    simhits_bh_token_(consumes<edm::PCaloHitContainer>(conf.getParameter<edm::InputTag>("bhSimHits"))),
    keV2fC_(conf.getParameter<double>("keV2fC")),
    fcPerMip_(conf.getParameter<std::vector<double>>("fcPerMip")),
    layerWeights_(conf.getParameter<std::vector<double>>("layerWeights")),
    thicknessCorrections_(conf.getParameter<std::vector<double>>("thicknessCorrections")) {

  produces<nanoaod::FlatTable>();
}

void TCTableProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions){
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("TriggerCells");
  desc.add<edm::InputTag>("MultiClusters");
  desc.add<edm::InputTag>("eeSimHits");
  desc.add<edm::InputTag>("fhSimHits");
  desc.add<edm::InputTag>("bhSimHits");
  desc.add<double>("keV2fC"),
  desc.add<std::vector<double>>("fcPerMip");
  desc.add<std::vector<double>>("layerWeights");
  desc.add<std::vector<double>>("thicknessCorrections");
  descriptions.addWithDefaultLabel(desc);
}

void TCTableProducer::produce(edm::Event& e, const edm::EventSetup& es){
  int tc_n_;
  std::vector<uint32_t> tc_id_;
  std::vector<int> tc_subdet_;
  std::vector<int> tc_side_;
  std::vector<int> tc_layer_;
  std::vector<int> tc_waferu_;
  std::vector<int> tc_waferv_;
  std::vector<int> tc_wafertype_;
  std::vector<int> tc_cellu_;
  std::vector<int> tc_cellv_;
  std::vector<uint32_t> tc_data_;
  std::vector<uint32_t> tc_uncompressedCharge_;
  std::vector<uint32_t> tc_compressedCharge_;
  std::vector<float> tc_mipPt_;
  std::vector<float> tc_pt_;
  std::vector<float> tc_energy_;
  std::vector<float> tc_simenergy_;
  std::vector<float> tc_eta_;
  std::vector<float> tc_phi_;
  std::vector<float> tc_x_;
  std::vector<float> tc_y_;
  std::vector<float> tc_z_;
  std::vector<uint32_t> tc_cluster_id_;
  std::vector<uint32_t> tc_multicluster_id_;
  std::vector<float> tc_multicluster_pt_;
  std::vector<int> tc_genparticle_index_;

  typedef edm::AssociationMap<edm::OneToMany<CaloParticleCollection, l1t::HGCalTriggerCellBxCollection>> CaloToCellsMap;

  // retrieve trigger cells
  edm::Handle<l1t::HGCalTriggerCellBxCollection> trigger_cells_h;
  e.getByToken(tc_token_, trigger_cells_h);
  const l1t::HGCalTriggerCellBxCollection& trigger_cells = *trigger_cells_h;

  // retrieve clusters
  edm::Handle<l1t::HGCalMulticlusterBxCollection> multiclusters_h;
  e.getByToken(multiclusters_token_, multiclusters_h);
  const l1t::HGCalMulticlusterBxCollection& multiclusters = *multiclusters_h;

  // retrieve geometry
  es.get<CaloGeometryRecord>().get(geometry_);

  // sim hit association
  std::unordered_map<uint32_t, double> simhits_ee;
  std::unordered_map<uint32_t, double> simhits_fh;
  std::unordered_map<uint32_t, double> simhits_bh;
  simhits(e, simhits_ee, simhits_fh, simhits_bh);

  // Associate cells to clusters
  std::unordered_map<uint32_t, uint32_t> cell2cluster;
  std::unordered_map<uint32_t, l1t::HGCalMulticlusterBxCollection::const_iterator> cell2multicluster;
  for (auto mcl_itr = multiclusters.begin(0); mcl_itr != multiclusters.end(0); mcl_itr++) {
    // loop on 2D clusters inside 3D clusters
    for (const auto& cl_ptr : mcl_itr->constituents()) {
      // loop on TC inside 2D clusters
      for (const auto& tc_ptr : cl_ptr.second->constituents()) {
        cell2cluster.emplace(tc_ptr.second->detId(), cl_ptr.second->detId());
        cell2multicluster.emplace(tc_ptr.second->detId(), mcl_itr);
      }
    }
  }

  triggerTools_.eventSetup(es);

  printf("there are %u trigger cells\n", trigger_cells.size());
  for (auto tc_itr = trigger_cells.begin(0); tc_itr != trigger_cells.end(0); tc_itr++) {
    if (tc_itr->hwPt() > -1) {
      auto cl_itr = cell2cluster.find(tc_itr->detId());
      auto mcl_itr = cell2multicluster.find(tc_itr->detId());
      uint32_t cl_id = (cl_itr != cell2cluster.end() ? cl_itr->second : 0);
      uint32_t mcl_id = (mcl_itr != cell2multicluster.end() ? mcl_itr->second->detId() : 0);
      float mcl_pt = (mcl_itr != cell2multicluster.end() ? mcl_itr->second->pt() : 0.);
      tc_n_++;
      // hardware data
      DetId id(tc_itr->detId());
      tc_id_.emplace_back(tc_itr->detId());
      tc_side_.emplace_back(triggerTools_.zside(id));
      tc_layer_.emplace_back(triggerTools_.layerWithOffset(id));
      if (id.det() == DetId::HGCalTrigger) {
        HGCalTriggerDetId idtrg(id);
        tc_subdet_.emplace_back(idtrg.subdet());
        tc_waferu_.emplace_back(idtrg.waferU());
        tc_waferv_.emplace_back(idtrg.waferV());
        tc_wafertype_.emplace_back(idtrg.type());
        tc_cellu_.emplace_back(idtrg.triggerCellU());
        tc_cellv_.emplace_back(idtrg.triggerCellV());
      } else if (id.det() == DetId::HGCalHSc) {
        HGCScintillatorDetId idsci(id);
        tc_subdet_.emplace_back(idsci.subdet());
        tc_waferu_.emplace_back(-999);
        tc_waferv_.emplace_back(-999);
        tc_wafertype_.emplace_back(idsci.type());
        tc_cellu_.emplace_back(idsci.ietaAbs());
        tc_cellv_.emplace_back(idsci.iphi());
      } else {
        throw cms::Exception("InvalidHGCalTriggerDetid")
            << "Found unexpected trigger cell detid to be filled in HGCal Trigger Cell ntuple.";
      }
      tc_data_.emplace_back(tc_itr->hwPt());
      tc_uncompressedCharge_.emplace_back(tc_itr->uncompressedCharge());
      tc_compressedCharge_.emplace_back(tc_itr->compressedCharge());
      tc_mipPt_.emplace_back(tc_itr->mipPt());
      // physical values
      tc_pt_.emplace_back(tc_itr->pt());
      tc_energy_.emplace_back(tc_itr->energy());
      tc_eta_.emplace_back(tc_itr->eta());
      tc_phi_.emplace_back(tc_itr->phi());
      tc_x_.emplace_back(tc_itr->position().x());
      tc_y_.emplace_back(tc_itr->position().y());
      tc_z_.emplace_back(tc_itr->position().z());
      // Links between TC and clusters
      tc_cluster_id_.emplace_back(cl_id);
      tc_multicluster_id_.emplace_back(mcl_id);
      tc_multicluster_pt_.emplace_back(mcl_pt);

      double energy = 0;
      unsigned layer = triggerTools_.layerWithOffset(id);
      // search for simhit for all the cells inside the trigger cell
      for (uint32_t c_id : geometry_->getCellsFromTriggerCell(id)) {
        int thickness = triggerTools_.thicknessIndex(c_id);
        if (triggerTools_.isEm(id)) {
          auto itr = simhits_ee.find(c_id);
          if (itr != simhits_ee.end())
            energy += calibrate(itr->second, thickness, layer);
        } else if (triggerTools_.isSilicon(id)) {
          auto itr = simhits_fh.find(c_id);
          if (itr != simhits_fh.end())
            energy += calibrate(itr->second, thickness, layer);
        } else {
          auto itr = simhits_bh.find(c_id);
          if (itr != simhits_bh.end())
            energy += itr->second;
        }
      }
      tc_simenergy_.emplace_back(energy);
    }
  }

  auto table = std::make_unique<nanoaod::FlatTable>(tc_id_.size(), "TCTable", false);
  table->addColumn<uint32_t>("id", tc_id_, "ID");
  table->addColumn<int>("subdet", tc_subdet_, "subdet");
  table->addColumn<int>("zside", tc_side_, "zside");
  table->addColumn<int>("layer", tc_layer_, "");
  table->addColumn<int>("waferu", tc_waferu_, "");
  table->addColumn<int>("waferv", tc_waferv_, "");

  e.put(std::move(table));
}

double TCTableProducer::calibrate(double energy, int thickness, unsigned layer) {
  double fcPerMip = fcPerMip_[thickness];
  double thicknessCorrection = thicknessCorrections_[thickness];
  double layerWeight = layerWeights_[layer];
  double TeV2GeV = 1.e3;
  return energy * keV2fC_ / fcPerMip * layerWeight * TeV2GeV / thicknessCorrection;
}

void TCTableProducer::simhits(const edm::Event& e,
                              std::unordered_map<uint32_t, double>& simhits_ee,
                              std::unordered_map<uint32_t, double>& simhits_fh,
                              std::unordered_map<uint32_t, double>& simhits_bh) {

  edm::Handle<edm::PCaloHitContainer> ee_simhits_h;
  e.getByToken(simhits_ee_token_, ee_simhits_h);
  const edm::PCaloHitContainer& ee_simhits = *ee_simhits_h;
  edm::Handle<edm::PCaloHitContainer> fh_simhits_h;
  e.getByToken(simhits_fh_token_, fh_simhits_h);
  const edm::PCaloHitContainer& fh_simhits = *fh_simhits_h;
  edm::Handle<edm::PCaloHitContainer> bh_simhits_h;
  e.getByToken(simhits_bh_token_, bh_simhits_h);
  const edm::PCaloHitContainer& bh_simhits = *bh_simhits_h;

  // EE
  for (const auto& simhit : ee_simhits) {
    DetId id = triggerTools_.simToReco(simhit.id(), geometry_->eeTopology());
    if (id.rawId() == 0)
      continue;
    auto itr_insert = simhits_ee.emplace(id, 0.);
    itr_insert.first->second += simhit.energy();
  }
  //  FH
  for (const auto& simhit : fh_simhits) {
    DetId id = triggerTools_.simToReco(simhit.id(), geometry_->fhTopology());
    if (id.rawId() == 0)
      continue;
    auto itr_insert = simhits_fh.emplace(id, 0.);
    itr_insert.first->second += simhit.energy();
  }
  //  BH
  for (const auto& simhit : bh_simhits) {
    DetId id = triggerTools_.simToReco(simhit.id(), geometry_->hscTopology());
    if (id.rawId() == 0)
      continue;
    auto itr_insert = simhits_bh.emplace(id, 0.);
    itr_insert.first->second += simhit.energy();
  }
}

DEFINE_FWK_MODULE(TCTableProducer);
