#include "PhysicsTools/NanoAOD/interface/ObjectIndexFromOneToManyQualAssociationProducer.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "SimDataFormats/CaloAnalysis/interface/SimClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectIndexFromOneToManyQualAssociationTableProducer<edm::View<CaloRecHit>, SimClusterCollection, float>
    CaloRecHitToSimClusterIndexTableProducer;

DEFINE_FWK_MODULE(CaloRecHitToSimClusterIndexTableProducer);
