import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import CandVars,Var,P3Vars
from DPGAnalysis.PFNanoAOD.pfClusters_cff import *
from DPGAnalysis.PFNanoAOD.pfCands_cff import *
from DPGAnalysis.CaloNanoAOD.hcalRecHits_cff import *

hitsAndElementsToPFCands = cms.EDProducer("PFCandAssociationsProducer",
    caloRecHits = cms.VInputTag("hbhereco", "hfreco", "horeco"),
    pfClusters = cms.VInputTag("particleFlowClusterECAL", "particleFlowClusterHCAL", "particleFlowClusterHF"),
    tracks = cms.VInputTag("generalTracks"),
    pfCands = cms.InputTag("particleFlow"),
)

pfClusterECALToCandTable = cms.EDProducer("PFClusterToPFCandIndexTableProducer",
    cut = ecalPFClusTable.cut,
    src = ecalPFClusTable.src,
    objName = ecalPFClusTable.name,
    branchName = pfCandTable.name,
    objMap = cms.InputTag(f"hitsAndElementsToPFCands:{ecalPFClusTable.src.value()}ToPFCand"),
    docString = cms.string("Index of PFCand containing PFCluster")
)

pfClusterHCALToCandTable = cms.EDProducer("PFClusterToPFCandIndexTableProducer",
    cut = hcalPFClusTable.cut,
    src = hcalPFClusTable.src,
    objName = hcalPFClusTable.name,
    branchName = pfCandTable.name,
    objMap = cms.InputTag(f"hitsAndElementsToPFCands:{hcalPFClusTable.src.value()}ToPFCand"),
    docString = cms.string("Index of PFCand containing PFCluster")
)

pfClusterHFToCandTable = cms.EDProducer("PFClusterToPFCandIndexTableProducer",
    cut = hfPFClusTable.cut,
    src = hfPFClusTable.src,
    objName = hfPFClusTable.name,
    branchName = pfCandTable.name,
    objMap = cms.InputTag(f"hitsAndElementsToPFCands:{hfPFClusTable.src.value()}ToPFCand"),
    docString = cms.string("Index of PFCand containing PFCluster")
)

#hbheRecHitToPFCandTable = cms.Producer("CaloRecHitToPFCandidate
hbheRecHitsToPFCandTable = cms.EDProducer("CaloRecHitToPFCandIndexTableProducer",
    cut = hbheRecHitTable.cut,
    src = hbheRecHitTable.src,
    objName = hbheRecHitTable.name,
    branchName = pfCandTable.name,
    objMap = cms.InputTag(f"hitsAndElementsToPFCands:{hbheRecHitTable.src.value()}ToPFCand"),
    docString = cms.string("MergedSimCluster responsible for most sim energy in RecHit DetId")
)

pfAssociationTables = cms.Sequence(
	hitsAndElementsToPFCands
	+pfClusterECALToCandTable
	+pfClusterHCALToCandTable
	+pfClusterHFToCandTable
 	+hbheRecHitsToPFCandTable
)

