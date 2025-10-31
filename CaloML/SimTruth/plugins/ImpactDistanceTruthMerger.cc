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
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "CommonTools/BaseParticlePropagator/interface/RawParticle.h"
#include "CommonTools/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "CaloML/SimTruth/src/util.h"

class ImpactDistanceTruthMerger : public edm::stream::EDProducer<> {
public:
    explicit ImpactDistanceTruthMerger(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;

private:
    double impactDistance(
            const SimTrack& track1,
            const SimTrack& track2,
            const SimVertex& vertex1,
            const SimVertex& vertex2);

    edm::EDGetToken simclusters_token_;
    edm::EDGetToken simvertices_token_;
    std::vector<edm::EDGetToken> simhits_tokens_;

    double caloR_;
    double caloZ_;
    edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magfield_token_;
    BaseParticlePropagator propagator_;
    double distanceTol_;

    int verbose_;
};


ImpactDistanceTruthMerger::ImpactDistanceTruthMerger(const edm::ParameterSet& conf)
    : simclusters_token_(consumes<std::vector<SimCluster>>(conf.getParameter<edm::InputTag>("simclusters"))),
      simvertices_token_(consumes<std::vector<SimVertex>>(conf.getParameter<edm::InputTag>("simvertices"))),
      caloR_(conf.getParameter<double>("caloR")),
      caloZ_(conf.getParameter<double>("caloZ")),
      magfield_token_(esConsumes<MagneticField, IdealMagneticFieldRecord, edm::Transition::BeginRun>()),
      distanceTol_(conf.getParameter<double>("distanceTol")),
      verbose_(conf.getParameter<int>("verbose")) 
{
    std::vector<edm::InputTag> simhits_tags = conf.getParameter<std::vector<edm::InputTag>>("simhits");
    for (const auto& tag : simhits_tags) {
        simhits_tokens_.emplace_back(consumes<edm::PCaloHitContainer>(tag));
    }

    produces<std::vector<SimCluster>>("mergedSimClusters");
    produces<std::vector<SimTrack>>("mergedSimTracks"); 

    propagator_.setPropagationConditions(
        caloR_, caloZ_
    );
}

void ImpactDistanceTruthMerger::beginRun(const edm::Run&, const edm::EventSetup& es) {
    edm::ESHandle<MagneticField> magfield;
    magfield = es.getHandle(magfield_token_);
    propagator_.setMagneticField(magfield->inTesla(GlobalPoint(0,0,0)).z());
}

double ImpactDistanceTruthMerger::impactDistance(
        const SimTrack& track1,
        const SimTrack& track2,
        const SimVertex& vertex1,
        const SimVertex& vertex2) {

    RawParticle track1P(
        track1.momentum().Px(),
        track1.momentum().Py(),
        track1.momentum().Pz(),
        track1.momentum().E(),
        track1.charge()
    );
    track1P.setVertex(
        vertex1.position().X(),
        vertex1.position().Y(),
        vertex1.position().Z(),
        0.0
    );
    propagator_.setParticle(track1P);
    propagator_.propagate();
    track1P = propagator_.particle();

    RawParticle track2P(
        track2.momentum().Px(),
        track2.momentum().Py(),
        track2.momentum().Pz(),
        track2.momentum().E(),
        track2.charge()
    );
    track2P.setVertex(
        vertex2.position().X(),
        vertex2.position().Y(),
        vertex2.position().Z(),
        0.0
    );
    propagator_.setParticle(track2P);
    propagator_.propagate();
    track2P = propagator_.particle();

    double dx = track1P.vertex().X() - track2P.vertex().X();
    double dy = track1P.vertex().Y() - track2P.vertex().Y();
    double dz = track1P.vertex().Z() - track2P.vertex().Z();
    double R = std::sqrt(dx*dx + dy*dy + dz*dz);
    return R;
}

void ImpactDistanceTruthMerger::produce(edm::Event& evt, const edm::EventSetup& es) {
    // Get the simhits (needed for energy maps)
    std::vector<edm::Handle<edm::PCaloHitContainer>> simhits_handles;
    simhits_handles.resize(simhits_tokens_.size());
    for (size_t i = 0; i < simhits_tokens_.size(); ++i) {
        evt.getByToken(simhits_tokens_[i], simhits_handles[i]);
    }

    //get the simclusters
    edm::Handle<std::vector<SimCluster>> simclusters_h;
    evt.getByToken(simclusters_token_, simclusters_h);
    const auto& simclusters = *simclusters_h;

    // Get the simvertices
    edm::Handle<std::vector<SimVertex>> simvertices_h;
    evt.getByToken(simvertices_token_, simvertices_h);
    const auto& simvertices = *simvertices_h;

    auto totalEnergies = CaloML::buildTotalEnergies(simhits_handles);

    std::vector<std::set<uint32_t>> adjacencies;
    adjacencies.resize(simclusters.size());
    for (size_t i=0; i<simclusters.size(); ++i){
        for(size_t j=i+1; j<simclusters.size(); ++j){
            // Only attempt distance-based merging if both clusters have a single generator particle
            if (simclusters[i].genParticles().size() == 1 && 
                simclusters[j].genParticles().size() == 1) {

                const auto& track1 = simclusters[i].g4Tracks()[0];
                const auto& track2 = simclusters[j].g4Tracks()[0];

                if (track1.vertIndex() < 0 || track2.vertIndex() < 0) continue;

                double R = impactDistance(
                    track1, track2,
                    simvertices[track1.vertIndex()],
                    simvertices[track2.vertIndex()]
                );

                if (R < distanceTol_) {
                    adjacencies[i].insert(j);
                    adjacencies[j].insert(i);
                }
            }
        }
    }

    std::vector<bool> visited(simclusters.size(), false);
    std::vector<std::vector<uint32_t>> components;
    //use BFS to find connected components
    for (size_t i = 0; i < simclusters.size(); ++i) {
        if (visited[i]) continue;

        std::vector<uint32_t> component;
        std::queue<uint32_t> queue;
        queue.push(i);
        visited[i] = true;

        while (!queue.empty()) {
            uint32_t current = queue.front();
            queue.pop();
            component.push_back(current);

            for (uint32_t neighbor : adjacencies[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            }
        }
        components.push_back(component);
    }

    if (verbose_){
        printf("Clusters:\n");
        for(size_t i=0; i<simclusters.size(); ++i){
            printf("Cluster %zu: ", i);
            printf("(%g, %g, %g), (%d; %f)",
                    simclusters[i].pt(),
                    simclusters[i].eta(),
                    simclusters[i].phi(),
                    simclusters[i].pdgId(),
                    simclusters[i].charge());
            printf("\n");
        }

        printf("Connected components:\n");
        for(const auto& comp : components){
            printf("Component: ");
            for(int idx : comp){
                printf("%d ", idx);
            }
            printf("\n");
        }
    }

    auto mergedClusters = std::make_unique<std::vector<SimCluster>>();
    auto mergedTracks = std::make_unique<std::vector<SimTrack>>();

    for (const auto& component : components) {
        if (component.size() == 1) {
            mergedClusters->push_back(simclusters[component[0]]);
            continue;
        }

        SimTrack mergedTrack = CaloML::mergeTracksFromComponent(simclusters, component);
        auto mergedHitEnergies = CaloML::computeMergedHitEnergiesForComponent(simclusters, component, totalEnergies);
        SimCluster newcluster = CaloML::makeMergedSimCluster(mergedTrack, mergedHitEnergies, totalEnergies);
        
        mergedClusters->push_back(newcluster);
        mergedTracks->push_back(mergedTrack);
    }

    evt.put(std::move(mergedClusters), "mergedSimClusters");
    evt.put(std::move(mergedTracks), "mergedSimTracks");
}

DEFINE_FWK_MODULE(ImpactDistanceTruthMerger);
