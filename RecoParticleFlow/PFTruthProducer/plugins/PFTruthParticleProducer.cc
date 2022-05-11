// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/global/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/Associations/interface/TrackToTrackingParticleAssociator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticleFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/SimClusterFwd.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "SimDataFormats/PFAnalysis/interface/PFTruthParticle.h"
#include "SimDataFormats/PFAnalysis/interface/PFTruthParticleFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"

#include "DataFormats/HGCRecHit/interface/HGCRecHit.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"

#include "FWCore/Utilities/interface/EDGetToken.h"
#include <set>

//
// class declaration
//
/*
 *
 *  produces in two steps PFTruthParticles.
 *  This producer considers truth particles one-by-one and does not
 *  account for possible overlaps. These will need to be resolved in a second producer
 *  (PFTruthParticleMerger).
 *  The advantage is that the plugin here can run on every single event, independent of
 *  pileup mixing and event density, while the second producer can be run after mixing in pileup
 *
 *  Step A)
 *     Produces an ideal PFTruthParticle, without considering detector limitations.
 *     This is done by connecting every trackingParticle and SimCluster that originated
 *     from a particle passed to Geant back to it.
 *     No configuration options needed.
 *     The tracking particles should be selected by one of the standard tracking selectors, e.g.:
 *     https://github.com/cms-sw/cmssw/blob/master/Validation/RecoTrack/python/TrackingParticleSelectionForEfficiency_cfi.py
 *
 *  Step B)
 *     The ideal PFTruthParticle gets split according to detector limitations, e.g. if
 *     a neutral pion splits to 2 photons and leaves to distinct photon showers,
 *     this step would produce 2 photons.
 *
 *     For neutral and charged particles, stray clusters with deltaR larger than <softkillcone>
 *     and tracks that contribute with less than a relative <softkillrelenergy> to
 *     the total particle energy are removed.
 *
 *     This splitting of neutral particles depends on the granularity of the SimClusters
 *     in the respective subdetector.
 *     <add other possible options as configuration parameters>
 *
 *     In case of charged particles, there are multiple options, e.g. when to split off bremsstrahlung
 *     for an electron.
 *
 *
 *
 *
 */

typedef edm::AssociationMap<edm::OneToMany<
    TrackingParticleCollection, SimClusterCollection>> TrackingParticleToSimCluster;

class PFTruthParticleProducer : public edm::global::EDProducer<> {
public:
  explicit PFTruthParticleProducer(const edm::ParameterSet &);
  ~PFTruthParticleProducer() override;

private:
  void produce(edm::StreamID, edm::Event &, const edm::EventSetup &) const override;

  const SimTrack* getRoot(const SimTrack * st,
          const std::vector<SimTrack> & simtracks,
          const std::vector<SimVertex> & simvertices,
          const std::map<int,int>&  trackIdToTrackIdxAsso ) const;

  edm::EDGetTokenT<TrackingParticleCollection> tpCollectionToken_;
  edm::EDGetTokenT<CaloParticleCollection> cpCollectionToken_;
  edm::EDGetTokenT<SimClusterCollection> scCollectionToken_;
  edm::EDGetTokenT<std::vector<SimVertex> > svCollectionToken_;
  edm::EDGetTokenT<std::vector<SimTrack> > stCollectionToken_;
  edm::EDGetTokenT<HGCRecHitCollection> caloRecHitToken_;
  edm::EDGetTokenT<edm::View<reco::Track>> tracksToken_;
  edm::EDGetTokenT<edm::Association<SimClusterCollection>> recHitToSCToken_;
  edm::EDGetTokenT<reco::SimToRecoCollection> tpToTrackToken_;

};

PFTruthParticleProducer::PFTruthParticleProducer(const edm::ParameterSet &pset)
    : tpCollectionToken_(consumes<TrackingParticleCollection>(pset.getParameter<edm::InputTag>("trackingParticles"))),
      cpCollectionToken_(consumes<CaloParticleCollection>(pset.getParameter<edm::InputTag>("caloParticles"))),
      scCollectionToken_(consumes<SimClusterCollection>(pset.getParameter<edm::InputTag>("simClusters"))),
      svCollectionToken_(consumes<std::vector<SimVertex> >(pset.getParameter<edm::InputTag>("simVertices"))),
      stCollectionToken_(consumes<std::vector<SimTrack> >(pset.getParameter<edm::InputTag>("simTracks"))),
      caloRecHitToken_(consumes<HGCRecHitCollection>(pset.getParameter<edm::InputTag>("caloRecHits"))),
      tracksToken_(consumes<edm::View<reco::Track>>(pset.getParameter<edm::InputTag>("tracks"))),
      recHitToSCToken_(consumes<edm::Association<SimClusterCollection> >(pset.getParameter<edm::InputTag>("rechitToSimClusAssoc"))),
      tpToTrackToken_(consumes<reco::SimToRecoCollection>(pset.getParameter<edm::InputTag>("trackingPartToTrackAssoc")))
{
  produces<PFTruthParticleCollection>();
  produces<edm::Association<PFTruthParticleCollection>>("trackingPartToPFTruth");
  produces<edm::Association<PFTruthParticleCollection>>("simClusToPFTruth");
  produces<edm::Association<PFTruthParticleCollection>>("caloRecHitToPFTruth");
  produces<edm::Association<PFTruthParticleCollection>>("trackToPFTruth");
}

PFTruthParticleProducer::~PFTruthParticleProducer() {}

//
// member functions
//


const SimTrack* PFTruthParticleProducer::getRoot(
        const SimTrack * st,
        const std::vector<SimTrack> & simtracks,
        const std::vector<SimVertex> & simvertices,
        const std::map<int,int>&  trackIdToTrackIdxAsso ) const{

    const SimTrack* out=st;
    int vidx = out->vertIndex();//starting point
    while(true){
        if(vidx<0)
            break; //no vertex
        const SimVertex& vertex = simvertices.at(vidx);
        int stid = vertex.parentIndex();//this is Geant track ID, not vector index
        if(stid < 0)
            break;
        int stidx = trackIdToTrackIdxAsso.at(stid); //get vector index
        if(stidx < 0)
            break;
        const SimTrack & simtrack = simtracks.at(stidx);
        vidx = simtrack.vertIndex();
        out=&simtrack;
    }
    return out;
}

class SimHistoryTool{
public:
    SimHistoryTool(const std::vector<SimTrack> & simtracks,
        const std::vector<SimVertex> & simvertices,
        const std::map<int,int>&  trackIdToTrackIdxAsso):
            simtracks_(simtracks),simvertices_(simvertices),trackIdToTrackIdxAsso_(trackIdToTrackIdxAsso)
        {}

    const SimTrack* getRoot(const SimTrack * st)const;
    const SimTrack* traverseStep(const SimTrack * st)const;

private:
   // SimHistoryTool(){}

    const std::vector<SimTrack> & simtracks_;
    const std::vector<SimVertex> & simvertices_;
    const std::map<int,int>&  trackIdToTrackIdxAsso_;
};

//use it above if needed
const SimTrack* SimHistoryTool::traverseStep(const SimTrack * st)const{

    const SimTrack* out=st;
    int vidx = out->vertIndex();
    if(vidx<0)
        return out; //no vertex
    const SimVertex& vertex = simvertices_.at(vidx);
    int stid = vertex.parentIndex();//this is Geant track ID, not vector index
    if(stid < 0)
        return out;//no parent
    int stidx = trackIdToTrackIdxAsso_.at(stid); //get vector index
    if(stidx < 0)
        return out;//id does not exist
    const SimTrack & simtrack = simtracks_.at(stidx);
    vidx = simtrack.vertIndex();
    out=&simtrack;
    return out;
}


const SimTrack* SimHistoryTool::getRoot(const SimTrack * st) const{
    const SimTrack* out=st;
    while(true){
        const SimTrack * test = traverseStep(out);
        if(test == out)//root
            break;
        out=test;
    }
    return out;
}

static std::vector<size_t> matchedSCtoTrackIdxs(const SimClusterRefVector &simClusters,
        const TrackingParticle& tp){

    //FIXME: this needs the actual logic

    for(const auto& t:tp.g4Tracks()){

    }

    //add more logic here
    std::vector<size_t>  out;
    for(const auto& sc: simClusters)
        out.push_back(sc.key());
    return out;

}


// ------------ method called to produce the data  ------------
void PFTruthParticleProducer::produce(edm::StreamID, edm::Event &iEvent, const edm::EventSetup &iSetup) const {

  edm::Handle<TrackingParticleCollection> tpCollection;
  iEvent.getByToken(tpCollectionToken_, tpCollection);

  edm::Handle<CaloParticleCollection> cpCollection;
  iEvent.getByToken(cpCollectionToken_, cpCollection);

  edm::Handle<SimClusterCollection> scCollection;
  iEvent.getByToken(scCollectionToken_, scCollection);

  edm::Handle<std::vector<SimVertex> > svCollection;
  iEvent.getByToken(svCollectionToken_, svCollection);

  edm::Handle<std::vector<SimTrack> > stCollection;
  iEvent.getByToken(stCollectionToken_, stCollection);

  edm::Handle<HGCRecHitCollection> caloRecHitCollection;
  iEvent.getByToken(caloRecHitToken_, caloRecHitCollection);

  edm::Handle<edm::View<reco::Track>> trackCollection;
  iEvent.getByToken(tracksToken_, trackCollection);

  edm::Handle<edm::Association<SimClusterCollection>> recHitToSC;
  iEvent.getByToken(recHitToSCToken_, recHitToSC);

  edm::Handle<reco::SimToRecoCollection> tpToTrack;
  iEvent.getByToken(tpToTrackToken_, tpToTrack);



  //index helpers
  std::map<int,int>  trackIdToTrackIdxAsso;
  for(size_t i=0;i<stCollection->size();i++){
      trackIdToTrackIdxAsso[stCollection->at(i).trackId()] = i;
  }

  SimHistoryTool histtool(*stCollection, *svCollection, trackIdToTrackIdxAsso);

  //match tracking to calo particles
  std::vector<std::vector<size_t> > cpToTpIdxAsso(cpCollection->size());
  for(size_t i_cp=0; i_cp < cpCollection->size(); i_cp++){
      const auto& cp = cpCollection->at(i_cp);

      for(size_t i_tp=0; i_tp< tpCollection->size(); i_tp++){

          const auto& tp = tpCollection->at(i_tp);
          auto tproot = histtool.getRoot(&tp.g4Tracks().at(0));

          if(tproot->trackId() == cp.g4Tracks().at(0).trackId()
                  && tproot->eventId() == cp.g4Tracks().at(0).eventId()){
              cpToTpIdxAsso.at(i_cp).push_back(i_tp);
          }
      }
  }

  //take only CPs with TP


  //best TP for each track & <-> one-to-one
  std::vector<int> tpToTrackIdxAsso(tpCollection->size(),-1);//fill with no asso
  std::vector<int> trackToTpIdxAsso(trackCollection->size(),-1);
  for(size_t i=0; i< tpCollection->size(); i++){
      TrackingParticleRef tp(tpCollection,i);

      if (tpToTrack->find(tp) != tpToTrack->end()) {
          auto& trackRefs = (*tpToTrack)[tp];
          auto& bestTrackRef = trackRefs.at(0).first;
          double bestQual = trackRefs.at(0).second;
          //possible quality requirement here

          //double relptdiff = (tp.pt() - bestTrackRef->pt())/tp.pt();
          //if(relptdiff>2. || relptdiff<0.5)
          //    continue; //just a test

          tpToTrackIdxAsso.at(i)=  bestTrackRef.key();
          trackToTpIdxAsso.at(bestTrackRef.key())=i;
      }
  }

  //take only CPs with TP with track, final association
  std::vector<std::vector<size_t> > trackToSCIdxAsso(trackCollection->size());
  std::vector<bool> scUsed(scCollection->size());

  for(size_t i_cp=0; i_cp < cpCollection->size(); i_cp++){
      for(const size_t i_tp: cpToTpIdxAsso.at(i_cp)){//tracking part idxs

          int i_track = tpToTrackIdxAsso.at(i_tp);
          if(i_track<0)
              continue;

          const auto tp = tpCollection->at(i_tp);
          auto matchedsc = matchedSCtoTrackIdxs(cpCollection->at(i_cp).simClusters(),tp );
          trackToSCIdxAsso.at(i_track)=matchedsc;
          for(const auto msc: matchedsc)
              scUsed.at(msc)=true;
      }
  }

  //create PF truth from trackingParticles with tracks


  auto PFtruth = std::make_unique<PFTruthParticleCollection>() ;

  std::vector<int> tpToPFpartIdx(tpCollection->size(), -1);
  std::vector<int> scToPFpartIdx(scCollection->size(), -1);
  std::vector<int> rhToPFpartIdx(caloRecHitCollection->size(), -1);
  std::vector<int> trackToPFpartIdx(trackCollection->size(), -1);

  //create charged PF truth
  for (size_t i_t = 0; i_t < trackToSCIdxAsso.size(); i_t++) {
      if(trackToSCIdxAsso.at(i_t).size()<1)
          continue;//not matched track

      int PFTIdx = PFtruth->size();

      trackToPFpartIdx.at(i_t) = PFTIdx;

      TrackingParticleRefVector tprefs;
      SimClusterRefVector screfs;

      //make the associations...
      tprefs.push_back(TrackingParticleRef(tpCollection,trackToTpIdxAsso.at(i_t)));
      tpToPFpartIdx.at(trackToTpIdxAsso.at(i_t)) = PFTIdx;

      for(const auto i_sc: trackToSCIdxAsso.at(i_t)){
          screfs.push_back(SimClusterRef(scCollection,i_sc));
          scToPFpartIdx.at(i_sc) = PFTIdx;
      }

      PFTruthParticle pftp(tprefs,screfs);
      //pftp.setCharge()
      //this needs to be refined
      //ID etc
      //pftp.setP4(tpCollection->at(trackToTpIdxAsso.at(i_t)).p4());
      //pftp.setVertex(tpCollection->at(trackToTpIdxAsso.at(i_t)).vertex());

      PFtruth->push_back(pftp);
  }

  //scMerger;
  //take the remaining SCs and merge them and make PFTruth out of them
  for(size_t i_sc=0;i_sc<scCollection->size();i_sc++){
      if(scUsed.at(i_sc)) continue;

      //scMerger.add(...)
  }
  std::vector<std::vector<size_t> > mergeIdxs;
  for(size_t i_sc=0;i_sc<scCollection->size();i_sc++){
      if(scUsed.at(i_sc)) continue;//FIXME not necessary later
      mergeIdxs.push_back({i_sc}); //FIXME for testing
  }

  for(const auto& mscs: mergeIdxs){

      int PFTIdx = PFtruth->size();

      SimClusterRefVector screfs;
      TrackingParticleRefVector tprefs;

      for(const auto i_sc: mscs){
          screfs.push_back(SimClusterRef(scCollection, i_sc));
          scToPFpartIdx.at(i_sc)=PFTIdx;
      }

      PFTruthParticle pftp(tprefs,screfs);
      pftp.setCharge(0);
      //ID etc

      PFtruth->push_back(pftp);
  }

  //assign rechits
  for (size_t i = 0; i <  caloRecHitCollection->size(); i++) {
      HGCRecHitRef rh(caloRecHitCollection, i);
      auto scRef = (*recHitToSC)[rh];
      int scIdx = scRef.isNonnull() ? scRef.key() : -1;
      int pfIdx = scIdx >= 0 ? scToPFpartIdx[scIdx] : -1;
      if (scIdx >= 0)
          rhToPFpartIdx.at(i) = pfIdx;
  }

  auto pfTruthHand = iEvent.put(std::move(PFtruth));

  // Used to fill the associations in the ntuples
  auto tpAssoc = std::make_unique<edm::Association<PFTruthParticleCollection>>(pfTruthHand);
  auto scAssoc = std::make_unique<edm::Association<PFTruthParticleCollection>>(pfTruthHand);
  auto rhAssoc = std::make_unique<edm::Association<PFTruthParticleCollection>>(pfTruthHand);
  auto trackAssoc = std::make_unique<edm::Association<PFTruthParticleCollection>>(pfTruthHand);

  edm::Association<PFTruthParticleCollection>::Filler tpFiller(*tpAssoc);
  tpFiller.insert(tpCollection, tpToPFpartIdx.begin(), tpToPFpartIdx.end());
  tpFiller.fill();

  edm::Association<PFTruthParticleCollection>::Filler scFiller(*scAssoc);
  scFiller.insert(scCollection, scToPFpartIdx.begin(), scToPFpartIdx.end());
  scFiller.fill();

  edm::Association<PFTruthParticleCollection>::Filler rhFiller(*rhAssoc);
  rhFiller.insert(caloRecHitCollection, rhToPFpartIdx.begin(), rhToPFpartIdx.end());
  rhFiller.fill();

  edm::Association<PFTruthParticleCollection>::Filler trackFiller(*trackAssoc);
  trackFiller.insert(trackCollection, trackToPFpartIdx.begin(), trackToPFpartIdx.end());
  trackFiller.fill();

  iEvent.put(std::move(tpAssoc), "trackingPartToPFTruth");
  iEvent.put(std::move(scAssoc), "simClusToPFTruth");
  iEvent.put(std::move(rhAssoc), "caloRecHitToPFTruth");
  iEvent.put(std::move(trackAssoc), "trackToPFTruth");
}

// define this as a plug-in
DEFINE_FWK_MODULE(PFTruthParticleProducer);

