import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.NanoAOD.genparticles_cff import genParticleTable
from PhysicsTools.NanoAOD.genVertex_cff import *
from DPGAnalysis.TrackNanoAOD.trackSimHits_cff import *
from DPGAnalysis.TrackNanoAOD.trackingParticles_cff import *
from DPGAnalysis.TrackNanoAOD.tracks_cff import *
from DPGAnalysis.PFNanoAOD.pfCands_cff import *
from DPGAnalysis.PFNanoAOD.pfTruth_cff import *
from DPGAnalysis.CaloNanoAOD.hcalRecHits_cff import *
from DPGAnalysis.CaloNanoAOD.hcalSimHits_cff import *

nanoMetadata = cms.EDProducer("UniqueStringProducer",
    strings = cms.PSet(
        tag = cms.string("untagged"),
    )
)

genParticleTable.src = "genParticles"
genParticleTable.variables = cms.PSet(genParticleTable.variables,
    charge = CandVars.charge)

CaloNanoAODSequence = cms.Sequence(
    nanoMetadata
    #+hgcRecHits #so that modules can use them
    +genVertexTable
    +genVertexT0Table
    +genParticleTable
    #+cms.Sequence(layerClusterTables)
    #+simTrackTables
    #+hgcSimHitsSequence
    # TODO: Fix producer and allow adding via configuration
    #+trackerSimHitTables
    #+simClusterTables
    +trackingParticleTables
    #+caloParticleTables
    +cms.Sequence(trackTables)
    #+mergedSimClusterTables
    +pfCandTable
    +hcalRecHitTables
    #+pfTruth
)

#cmsDriver complains if this doesn't exist I guess
def nanoAOD_customizeMC(process):
    process.CaloNanoAODSequence.insert(-1, hcalSimHitTables)
    return process
