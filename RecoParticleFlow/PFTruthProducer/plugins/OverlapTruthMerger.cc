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

class OverlapTruthMerger : public edm::stream::EDProducer<> {
public:
    explicit OverlapTruthMerger(const edm::ParameterSet&);
    
    void produce(edm::Event&, const edm::EventSetup&) override;
    void beginRun(const edm::Run&, const edm::EventSetup&) override;

private:
    double computeOverlap(
            const std::unordered_map<uint32_t, double>& cluster1,
            const std::unordered_map<uint32_t, double>& cluster2,
            const std::set<uint32_t>& detIds1,
            const std::set<uint32_t>& detIds2);
            
    double impactDistance(
            const SimTrack& track1,
            const SimTrack& track2,
            const SimVertex& vertex1,
            const SimVertex& vertex2);

    edm::EDGetToken simtracks_token_;
    edm::EDGetToken simclusters_token_;
    edm::EDGetToken simvertices_token_;
    std::vector<edm::EDGetToken> simhits_tokens_;

    double overlapThreshold_;

    double caloR_;
    double caloZ_;
    edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magfield_token_;
    BaseParticlePropagator propagator_;
    double distanceTol_;

    int verbose_;
};


OverlapTruthMerger::OverlapTruthMerger(const edm::ParameterSet& conf)
    : simtracks_token_(consumes<std::vector<SimTrack>>(conf.getParameter<edm::InputTag>("simtracks"))),
      simclusters_token_(consumes<std::vector<SimCluster>>(conf.getParameter<edm::InputTag>("simclusters"))),
      simvertices_token_(consumes<std::vector<SimVertex>>(conf.getParameter<edm::InputTag>("simvertices"))),
      overlapThreshold_(conf.getParameter<double>("overlapThreshold")),
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

void OverlapTruthMerger::beginRun(const edm::Run&, const edm::EventSetup& es) {
    edm::ESHandle<MagneticField> magfield;
    magfield = es.getHandle(magfield_token_);
    propagator_.setMagneticField(magfield->inTesla(GlobalPoint(0,0,0)).z());
}

double OverlapTruthMerger::computeOverlap(
        const std::unordered_map<uint32_t, double>& cluster1,
        const std::unordered_map<uint32_t, double>& cluster2,
        const std::set<uint32_t>& detIds1,
        const std::set<uint32_t>& detIds2) {

    std::set<uint32_t> commonDetIds;
    std::set_intersection(
        detIds1.begin(), detIds1.end(),
        detIds2.begin(), detIds2.end(),
        std::inserter(commonDetIds, commonDetIds.begin())
    );
    double overlapEnergy1 = 0.0;
    double overlapEnergy2 = 0.0;
    for (uint32_t detId : commonDetIds) {
        overlapEnergy1 += cluster1.at(detId);
        overlapEnergy2 += cluster2.at(detId);
    }

    double totalEnergy1 = 0.0;
    for (const auto& kvpair : cluster1) {
        totalEnergy1 += kvpair.second;
    }

    double totalEnergy2 = 0.0;
    for (const auto& kvpair : cluster2) {
        totalEnergy2 += kvpair.second;
    }

    return std::max(
        overlapEnergy1 / totalEnergy1, 
        overlapEnergy2 / totalEnergy2
    );
}

double OverlapTruthMerger::impactDistance(
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

void OverlapTruthMerger::produce(edm::Event& evt, const edm::EventSetup& es) {
    printf("test...\n");
    // Get the simtracks
    edm::Handle<std::vector<SimTrack>> simtracks_h;
    evt.getByToken(simtracks_token_, simtracks_h);
    const auto& simtracks = *simtracks_h;

    // Get the simhits
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

    std::unordered_map<uint32_t, double> totalEnergies;
    for (const auto& simhits_h : simhits_handles){
        for (const auto& simhit : *simhits_h){
            totalEnergies[simhit.id()] += simhit.energy();
        }
    }

    std::unordered_map<uint32_t, uint32_t> geantToIndexMap;
    for(size_t i = 0; i < simtracks.size(); ++i){
        geantToIndexMap[simtracks[i].trackId()] = i;
    }

    std::vector<std::unordered_map<uint32_t, double>> clusterHitEnergies;
    clusterHitEnergies.reserve(simclusters.size());
    std::vector<std::set<uint32_t>> clusterDetIds;
    clusterDetIds.reserve(simclusters.size());
    for(const auto& simcluster : simclusters){
        std::unordered_map<uint32_t, double> hitEnergies;
        std::set<uint32_t> detIds;
        for(const auto& hit : simcluster.hits_and_fractions()){
            hitEnergies[hit.first] += hit.second * totalEnergies[hit.first];
            detIds.insert(hit.first);
        }
        clusterHitEnergies.push_back(hitEnergies);
        clusterDetIds.push_back(detIds);
    }

    std::vector<std::set<uint32_t>> adjacencies;
    adjacencies.resize(simclusters.size());
    for (size_t i=0; i<simclusters.size(); ++i){
        for(size_t j=i+1; j<simclusters.size(); ++j){
            double overlap = computeOverlap(
                clusterHitEnergies[i], clusterHitEnergies[j],
                clusterDetIds[i], clusterDetIds[j]
            );

            if (overlap > overlapThreshold_){
                adjacencies[i].insert(j);
                adjacencies[j].insert(i);
            } else if (simclusters[i].genParticles().size() == 1 && 
                       simclusters[j].genParticles().size() == 1) {

                const auto& track1 = simclusters[i].g4Tracks()[0];
                const auto& track2 = simclusters[j].g4Tracks()[0];

                if (track1.vertIndex() < 0 ||
                    track2.vertIndex() < 0){
                    continue;
                }

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
    
        std::vector<SimTrack> tracks;
        for (uint32_t idx: component){
            for (const auto& track : simclusters[idx].g4Tracks()){
                tracks.push_back(track);
            }
        }

        std::sort(tracks.begin(), tracks.end(), [](const SimTrack& a, const SimTrack& b){
            return a.momentum().pt() > b.momentum().pt();
        });

        math::XYZTLorentzVectorD netmomentum = tracks[0].momentum();
        float netcharge = tracks[0].charge();
        int netpdgId = tracks[0].type();
        for(size_t i=1; i<tracks.size(); ++i){
            if (tracks[i].type() != netpdgId && netpdgId != 0){
                int smaller = std::min(netpdgId, tracks[i].type());
                int larger = std::max(netpdgId, tracks[i].type());

                if (larger == 22 && std::abs(smaller) == 11){
                    netpdgId = 11;
                } else if (larger == 22 && std::abs(smaller) == 13){
                    netpdgId = 13;
                } else if (smaller == -11 && larger == 11){
                    netpdgId = 22;
                } else if (smaller == -13 && larger == 13){
                    netpdgId = 22;
                } else {
                    printf("Warning: Merging tracks with different PDG IDs: %d and %d\n",
                           netpdgId, tracks[i].type());
                    netpdgId = 0;
                }
            }
            netmomentum += tracks[i].momentum();
            netcharge += tracks[i].charge();
        }
        SimTrack mergedTrack(
            netpdgId, netmomentum
        );

        std::unordered_map<uint32_t, double> mergedHitEnergies;
        for (uint32_t idx : component){
            for (const auto& hit : simclusters[idx].hits_and_fractions()){
                mergedHitEnergies[hit.first] += hit.second * totalEnergies[hit.first];
            }
        }

        SimCluster newcluster(mergedTrack);
        for (const auto& kvpair : mergedHitEnergies){
            printf("In OverlapTruthMerger, adding hit %u with energy %f\n",
                  kvpair.first, kvpair.second);
            newcluster.addRecHitAndFraction(
                    kvpair.first, 
                    kvpair.second / totalEnergies[kvpair.first]
            );
            newcluster.addHitEnergy(kvpair.second);
            newcluster.addSimHit(PCaloHit(float(kvpair.second)));
        }

        mergedClusters->push_back(newcluster);
        mergedTracks->push_back(mergedTrack);
    }

    evt.put(std::move(mergedClusters), "mergedSimClusters");
    evt.put(std::move(mergedTracks), "mergedSimTracks");
}

DEFINE_FWK_MODULE(OverlapTruthMerger);
