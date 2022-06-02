import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var,P3Vars

hbheRecHitsTable = cms.EDProducer("SimpleCaloRecHitFlatTableProducer",
    src = cms.InputTag("hbhereco"),
    cut = cms.string(""), 
    name = cms.string("RecHitHBHE"),
    doc  = cms.string("HCAL RecHits"),
    singleton = cms.bool(False), # the number of entries is variable
    extension = cms.bool(False), # this is the main table for the muons
    variables = cms.PSet(
        detId = Var('detid().rawId()', 'int', precision=-1, doc='detId'),
        energy = Var('energy', 'float', precision=14, doc='energy'),
        time = Var('time', 'float', precision=14, doc='hit time'),
    )
)

hbheRecHitPositionTable = cms.EDProducer("HCALRecHitPositionTableProducer",
    src = hbheRecHitsTable.src,
    cut = hbheRecHitsTable.cut, 
    name = hbheRecHitsTable.name,
    doc  = hbheRecHitsTable.doc,
)

hcalRecHitTables = cms.Sequence(
	hbheRecHitsTable
	+hbheRecHitPositionTable 
)
