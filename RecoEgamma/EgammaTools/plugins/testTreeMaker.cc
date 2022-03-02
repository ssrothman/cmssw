#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"

#include "TFile.h"
#include "TTree.h"

#include <string>

namespace {
  template<typename T> 
  const T* matchBySCSeedId(unsigned int seedId,const std::vector<T>& objs){
    for(auto& obj : objs){
      if(obj.superCluster()->seed()->seed().rawId()==seedId) return &obj;
    }
    return nullptr;
  }

  const pat::Photon* matchPho(unsigned int seedId,const std::vector<pat::Photon>& phos){
    return matchBySCSeedId(seedId,phos);
  }

  const reco::SuperCluster* matchSC(float eta,float phi,const std::vector<edm::Handle<reco::SuperClusterCollection> >& scHandles)
  {
    const reco::SuperCluster* bestMatch=nullptr;
    float bestDR2=0.3*0.3;
    for(const auto& scHandle : scHandles){
      for(const auto& sc: *scHandle){
        float dR2 = reco::deltaR2(sc.eta(),sc.phi(),eta,phi);
        if(dR2<bestDR2){
    bestMatch = &sc;
    bestDR2 = dR2;
        }
      }
    }
    return bestMatch;
  }
};

namespace {
  template<typename T> 
  edm::Handle<T> getHandle(const edm::Event& iEvent,const edm::EDGetTokenT<T>& token)
  {
    edm::Handle<T> handle;
    iEvent.getByToken(token,handle);
    return handle;
  }
  template<typename T> 
  std::vector<edm::Handle<T> > getHandle(const edm::Event& iEvent,const std::vector<edm::EDGetTokenT<T> >& tokens)
  {
    std::vector<edm::Handle<T> > handles;
    for(auto& token : tokens){
      edm::Handle<T> handle;
      iEvent.getByToken(token,handle);
      handles.emplace_back(std::move(handle));
    }
    return handles;
  }
};

class testTreeMaker : public edm::EDAnalyzer {

  private:
    TTree* tree;
    edm::EDGetTokenT<std::vector<pat::Photon>> photonsToken;
    edm::EDGetTokenT<reco::GenParticleCollection> genPartsToken;
    std::vector<edm::EDGetTokenT<reco::SuperClusterCollection>> scTokens_;
    float DRNenergy, BDTenergy, GENenergy;
    //tokens...
  
  public:
    explicit testTreeMaker(const edm::ParameterSet& params);

  private:
    virtual void beginJob();
    virtual void beginRun(const edm::Run& run, const edm::EventSetup& setup);
    virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
    virtual void endRun(edm::Run const* run, edm::EventSetup const*){}
    virtual void endJob(){};

    template<typename T>
    void setToken(edm::EDGetTokenT<T>& token,const edm::ParameterSet& iPara,const std::string& tagName){
      token = consumes<T>(iPara.getParameter<edm::InputTag>(tagName));
    }
    template<typename T>
    void setToken(std::vector<edm::EDGetTokenT<T> > & tokens,const edm::ParameterSet& iPara,const std::string& tagName){
      for(auto& tag: iPara.getParameter<std::vector<edm::InputTag> >(tagName)){
        tokens.push_back(consumes<T>(tag));
      }
    }
};

testTreeMaker::testTreeMaker(const edm::ParameterSet& params):
  tree(nullptr)
{
  setToken(photonsToken, params, "photonsTag");
  setToken(genPartsToken, params, "genPartsTag");
  setToken(scTokens_,params,"scTag");
}

void testTreeMaker::beginJob(){
  edm::Service<TFileService> fs;
  fs->file().cd();
  tree = new TTree("tree","");

  tree->Branch("DRNenergy",&DRNenergy,"DRNenergy/F");
  tree->Branch("BDTenergy",&BDTenergy,"BDTenergy/F");
  tree->Branch("GENenergy",&GENenergy,"GENenergy/F");
}

void testTreeMaker::beginRun(const edm::Run& run, const edm::EventSetup& setup){}

void testTreeMaker::analyze(const edm::Event& event, const edm::EventSetup& setup){
  auto photonsHandle = getHandle(event, photonsToken);
  auto genPartsHandle = getHandle(event,genPartsToken);
  auto scHandles = getHandle(event,scTokens_);

  for (const auto& genPart : *genPartsHandle){
    if((std::abs(genPart.pdgId())==11 || genPart.pdgId()==22) && genPart.statusFlags().isPrompt() && genPart.statusFlags().isFirstCopy()){

      const reco::SuperCluster* sc = matchSC(genPart.eta(),genPart.phi(),scHandles);
      if(!sc){
        continue;
      }
      const pat::Photon* pho = matchPho(sc->seed()->seed().rawId(),*photonsHandle);
      if(!pho){
        continue;
      }

      DRNenergy = pho->getCorrectedEnergy(reco::Photon::P4type::regression2);
      BDTenergy = pho->getCorrectedEnergy(reco::Photon::P4type::regression1);
      GENenergy = genPart.energy();

      //std::cout << "DRN" << DRNenergy << std::endl <<
      //             "BDT" << BDTenergy << std::endl <<
      //             "GEN" << GENenergy << std::endl << std::endl;

      tree->Fill();
    }
  }
};

DEFINE_FWK_MODULE(testTreeMaker);
