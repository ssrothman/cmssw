import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var,P3Vars

hcalSimHitTable = cms.EDProducer("SimplePCaloHitFlatTableProducer",
    src = cms.InputTag("g4SimHits:HcalHits"),
    cut = cms.string(""), 
    name = cms.string("SimHitHBHE"),
    doc  = cms.string("HCAL barrel and endcap sim hits"),
    singleton = cms.bool(False), # the number of entries is variable
    extension = cms.bool(False), # this is the main table 
    variables = cms.PSet(
        detId = Var('id', 'int', precision=-1, doc='detId'),
        energy = Var('energy', 'float', precision=14, doc='energy'),
        time = Var('time', 'float', precision=14, doc='hit time'),
        trackId = Var('geantTrackId', 'int', precision=-1, doc="Geant4 track ID")
    )
)

hcalSimHitPositionTable = cms.EDProducer("PCaloHitPositionTableProducer",
    src = hcalSimHitTable.src,
    cut = hcalSimHitTable.cut, 
    name = hcalSimHitTable.name,
    doc  = hcalSimHitTable.doc,
)

hcalSimHitTables = cms.Sequence(
    hcalSimHitTable
    +hcalSimHitPositionTable
)
