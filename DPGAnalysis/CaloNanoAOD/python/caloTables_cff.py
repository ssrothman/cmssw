import FWCore.ParameterSet.Config as cms

from DPGAnalysis.CaloNanoAOD.ecalSimHitTables_cff import *
from DPGAnalysis.CaloNanoAOD.hcalSimHitTables_cff import *
from DPGAnalysis.CaloNanoAOD.ecalRecHitTables_cff import *
from DPGAnalysis.CaloNanoAOD.hcalRecHitTables_cff import *

caloSimHitTables = cms.Sequence(ecalSimHitTables + hcalSimHitTables)
caloPFRecHitTables = cms.Sequence(ecalPFRecHitTables + hcalPFRecHitTables)
caloRecHitTables = cms.Sequence(ecalRecHitTables + hcalRecHitTables)
