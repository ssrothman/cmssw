import FWCore.ParameterSet.Config as cms

from PhysicsTools.NanoAOD.common_cff import Var,P3Vars

ecalRecHitEBTable = cms.EDProducer("SimpleCaloRecHitFlatTableProducer",
    src = cms.InputTag("EcalRecHitsEB"),
    cut = cms.string(''),
    name = cms.string('EcalRecHitEB'),
    doc = cms.string('Ecal EB RecHit table'),
    singleton = cms.bool(False),
    extension = cms.bool(False),
    variables = cms.PSet(
        detid = Var("detId()", 'int', doc = "detId"),
        energy = Var("energy", 'float', doc = "energy"),
        time = Var("time", 'float', doc = "time"),
    )
)

ecalRecHitEBPositionTable = cms.EDProducer("CaloRecHitPositionTableProducer",
    src = ecalRecHitEBTable.src,
    doc = ecalRecHitEBTable.doc,
    name = ecalRecHitEBTable.name,
    cut = ecalRecHitEBTable.cut,
)

ecalRecHitEETable = cms.EDProducer("SimpleCaloRecHitFlatTableProducer",
    src = cms.InputTag("EcalRecHitsEE"),
    cut = cms.string(''),
    name = cms.string('EcalRecHitEE'),
    doc = cms.string('Ecal EE RecHit table'),
    singleton = cms.bool(False),
    extension = cms.bool(False),
    variables = cms.PSet(
        detid = Var("detId()", 'int', doc = "detId"),
        energy = Var("energy", 'float', doc = "energy"),
        time = Var("time", 'float', doc = "time"),
    )
)

ecalRecHitEEPositionTable = cms.EDProducer("CaloRecHitPositionTableProducer",
    src = ecalRecHitEETable.src,
    doc = ecalRecHitEETable.doc,
    name = ecalRecHitEETable.name,
    cut = ecalRecHitEETable.cut,
)

ecalRecHitESTable = cms.EDProducer("SimpleCaloRecHitFlatTableProducer",
    src = cms.InputTag("EcalRecHitsES"),
    cut = cms.string(''),
    name = cms.string('EcalRecHitES'),
    doc = cms.string('Ecal ES RecHit table'),
    singleton = cms.bool(False),
    extension = cms.bool(False),
    variables = cms.PSet(
        detid = Var("detId()", 'int', doc = "detId"),
        energy = Var("energy", 'float', doc = "energy"),
        time = Var("time", 'float', doc = "time"),
    )
)

ecalRecHitESPositionTable = cms.EDProducer("PFRecHitPositionTableProducer",
    src = ecalRecHitESTable.src,
    doc = ecalRecHitESTable.doc,
    name = ecalRecHitESTable.name,
    cut = ecalRecHitESTable.cut,
)

ecalRecHitTables = cms.Sequence(ecalRecHitESTable
                                +ecalRecHitESPositionTable
                                +ecalRecHitEBTable
                                +ecalRecHitEBPositionTable
                                +ecalRecHitEETable
                                +ecalRecHitEEPositionTable)
