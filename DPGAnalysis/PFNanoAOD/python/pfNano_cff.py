import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.NanoAOD.genparticles_cff import genParticleTable
from PhysicsTools.NanoAOD.genVertex_cff import *
from DPGAnalysis.TrackNanoAOD.trackingParticles_cff import *
from DPGAnalysis.TrackNanoAOD.tracks_cff import *
from DPGAnalysis.PFNanoAOD.pfCands_cff import *
#from DPGAnalysis.PFNanoAOD.pfTruth_cff import *
from DPGAnalysis.CaloNanoAOD.caloParticles_cff import *
from DPGAnalysis.CaloNanoAOD.simClusters_cff import *

nanoMetadata = cms.EDProducer("UniqueStringProducer",
    strings = cms.PSet(
        tag = cms.string("untagged"),
    )
)

genParticleTable.src = "genParticles"
genParticleTable.variables = cms.PSet(genParticleTable.variables,
    charge = CandVars.charge)

pfNanoSequence = cms.Sequence(nanoMetadata
    +genVertexTable+genVertexT0Table+genParticleTable
    +caloParticleTables
    +simClusterTables
    +trackingParticleTables
    +caloParticleTables
    +pfCandTable
    #+pfTruth
    +trackTables
)
