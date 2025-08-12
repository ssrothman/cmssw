import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var

import SimCalorimetry.HGCalSimProducers.hgcalDigitizer_cfi as digiparam
import RecoLocalCalo.HGCalRecProducers.HGCalUncalibRecHit_cfi as recoparam
import RecoLocalCalo.HGCalRecProducers.HGCalRecHit_cfi as recocalibparam
import L1Trigger.L1THGCal.hgcalLayersCalibrationCoefficients_cfi as layercalibparam
fcPerMip = recoparam.HGCalUncalibRecHit.HGCEEConfig.fCPerMIP
keV2fC = digiparam.hgceeDigitizer.digiCfg.keV2fC
thicknessCorrections = recocalibparam.HGCalRecHit.thicknessCorrection

L1HGCalTCsTruth = cms.EDProducer("L1TriggerCellTruthProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    simHitsEE = cms.InputTag('g4SimHits','HGCHitsEE'),
    simHitsFH = cms.InputTag('g4SimHits','HGCHitsHEfront'),
    simHitsBH = cms.InputTag('g4SimHits','HGCHitsHEback'),
    simClusters = cms.InputTag('mix','MergedCaloTruth'),
    fcPerMip = fcPerMip,
    keV2fC = keV2fC,
    layerWeights = layercalibparam.triggerWeights.weights,
    thicknessCorrections = thicknessCorrections,
)

triggerCellTable = cms.EDProducer("SimpleHGCalTriggerCellFlatTableProducer",
    src = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    name = cms.string("L1THGCalTC"),
    doc = cms.string("L1THGCal Trigger Cells"),
    singleton = cms.bool(False),  # the number of entries is variable
    extension = cms.bool(False),  # this is the main table for the trigger cells
    variables = cms.PSet(
        detId = Var('detId()', 'int', doc='DetId of the trigger cell'),
        hwPt = Var('hwPt()', 'int', doc='Hardware pT of the trigger cell'),
        mipPt = Var('mipPt()', 'int', doc='MIP pT of the trigger cell'),
        pt = Var('pt()', 'float', doc='pT of the trigger cell'),
        energy = Var('energy()', 'float', doc='Energy of the trigger cell'),
    )
)

triggerCellPropsTable = cms.EDProducer("L1HGCalTCPropertiesTableProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    name = cms.string("L1THGCalTC"),
)

triggerCellTruthTable = cms.EDProducer("L1HGCalTCTruthTableProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    tcTruth = cms.InputTag('L1HGCalTCsTruth'),
    name = cms.string("L1THGCalTC"),
)
