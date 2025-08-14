import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var

import SimCalorimetry.HGCalSimProducers.hgcalDigitizer_cfi as digiparam
import RecoLocalCalo.HGCalRecProducers.HGCalUncalibRecHit_cfi as recoparam
import RecoLocalCalo.HGCalRecProducers.HGCalRecHit_cfi as recocalibparam
import L1Trigger.L1THGCal.hgcalLayersCalibrationCoefficients_cfi as layercalibparam
fcPerMip = recoparam.HGCalUncalibRecHit.HGCEEConfig.fCPerMIP
keV2fC = digiparam.hgceeDigitizer.digiCfg.keV2fC
thicknessCorrections = recocalibparam.HGCalRecHit.thicknessCorrection

#geometry from https://cms-docdb.cern.ch/cgi-bin/PublicDocDB/RetrieveFile?docid=13251&filename=20210803%20HGCAL%20PARAMETER%20DRAWING.pdf&version=11
simonMergedTruth = cms.EDProducer("SimTreeTruthMerger",
    simtracks = cms.InputTag('g4SimHits'),
    simvertices = cms.InputTag('g4SimHits'),
    simhits = cms.VInputTag('g4SimHits:HGCHitsEE',
                            'g4SimHits:HGCHitsHEfront',
                            'g4SimHits:HGCHitsHEback'),
    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),
    verbose = cms.int32(0)
)

testMerger = cms.EDProducer("OverlapTruthMerger",
    simtracks = cms.InputTag("simonMergedTruth:mergedSimTracks"),
    simvertices = cms.InputTag("g4SimHits"),
    simhits = cms.InputTag("simonMergedTruth:relabeledSimHits"),
    simclusters = cms.InputTag("simonMergedTruth:mergedSimClusters"),
    overlapThreshold = cms.double(0.2),
    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),
    distanceTol = cms.double(0.1),
    verbose = cms.int32(0)
)

SimonMergedSimClusterTable = cms.EDProducer("SimpleSimClusterFlatTableProducer",
    src = cms.InputTag('simonMergedTruth', 'mergedSimClusters'),
    name = cms.string("SimonMergedSimCluster"),
    extension = cms.bool(False),  # this is the main table for the simclusters
    singleton = cms.bool(False),  # the number of entries is variable
    variables = cms.PSet(
        eta = Var('eta()', 'float', doc='eta of the simcluster'),
        phi = Var('phi()', 'float', doc='phi of the simcluster'),
        energy = Var('energy()', 'float', doc='energy of the simcluster'),
    )
)

L1HGCalTCsTruth = cms.EDProducer("L1TriggerCellTruthProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    simHitsEE = cms.InputTag('g4SimHits','HGCHitsEE'),
    simHitsFH = cms.InputTag('g4SimHits','HGCHitsHEfront'),
    simHitsBH = cms.InputTag('g4SimHits','HGCHitsHEback'),
    simClusters = cms.InputTag('simonMergedTruth', 'mergedSimClusters'),
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


