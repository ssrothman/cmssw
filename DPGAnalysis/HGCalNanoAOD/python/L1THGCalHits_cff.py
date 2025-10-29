import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var

import SimCalorimetry.HGCalSimProducers.hgcalDigitizer_cfi as digiparam
import RecoLocalCalo.HGCalRecProducers.HGCalUncalibRecHit_cfi as recoparam
import RecoLocalCalo.HGCalRecProducers.HGCalRecHit_cfi as recocalibparam
import L1Trigger.L1THGCal.hgcalLayersCalibrationCoefficients_cfi as layercalibparam
fcPerMip = recoparam.HGCalUncalibRecHit.HGCEEConfig.fCPerMIP
keV2fC = digiparam.hgceeDigitizer.digiCfg.keV2fC
thicknessCorrections = recocalibparam.HGCalRecHit.thicknessCorrection

TCSimHits = cms.EDProducer("TriggerCellSimHitsProducer",
    simHits = cms.VInputTag('g4SimHits:HGCHitsEE',
                            'g4SimHits:HGCHitsHEfront',
                            'g4SimHits:HGCHitsHEback'),
)

#geometry from https://cms-docdb.cern.ch/cgi-bin/PublicDocDB/RetrieveFile?docid=13251&filename=20210803%20HGCAL%20PARAMETER%20DRAWING.pdf&version=11
simTreeMerger = cms.EDProducer("SimTreeTruthMerger",
    simtracks = cms.InputTag('g4SimHits'),
    simvertices = cms.InputTag('g4SimHits'),
    simhits = cms.VInputTag('TCSimHits'),

    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),

    verbose = cms.int32(0)
)

overlapMerger = cms.EDProducer("OverlapTruthMerger",
    simtracks = cms.InputTag("simTreeMerger:mergedSimTracks"),
    simvertices = cms.InputTag("g4SimHits"),
    simhits = cms.VInputTag("simTreeMerger:relabeledSimHits"),
    simclusters = cms.InputTag("simTreeMerger:mergedSimClusters"),

    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),

    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.0),

    verbose = cms.int32(0)
)

SimonMergedSimClusterTable = cms.EDProducer("SimpleSimClusterFlatTableProducer",
    src = cms.InputTag('overlapMerger', 'mergedSimClusters'),
    name = cms.string("SimonMergedSimCluster"),
    extension = cms.bool(False),  # this is the main table for the simclusters
    singleton = cms.bool(False),  # the number of entries is variable
    variables = cms.PSet(
        pt = Var('pt()', 'float', doc='pt of the simcluster'),
        eta = Var('eta()', 'float', doc='eta of the simcluster'),
        phi = Var('phi()', 'float', doc='phi of the simcluster'),
        energy = Var('energy()', 'float', doc='energy of the simcluster'),
        pdgId = Var('pdgId()', 'int', doc='PDG ID of the simcluster'),
        threeCharge = Var('threeCharge()', 'int', doc='3*charge of the simcluster'),
        nSimHits = Var('numberOfRecHits', 'int', doc='number of simhits in the simcluster'),
        hasHGCALHit = Var('hasHGCALHit()', 'bool', doc='whether the simcluster has a HGCAL hit'),
        allHitsHGCAL = Var('allHitsHGCAL()', 'bool', doc='whether all hits in the simcluster are HGCAL hits'),
        simEnergy = Var('simEnergy()', 'float', doc='simulated energy of the simcluster'),
    )
)

SimClusterTCTable = cms.EDProducer("SimClusterToTriggerCellTableProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    simClusters = cms.InputTag('overlapMerger', 'mergedSimClusters'),
    name = cms.string("SimonMergedSimCluster"),
    matchName = cms.string("SimonMergedSimClusterTCs")
)

L1HGCalTCsTruth = cms.EDProducer("L1TriggerCellTruthProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    simHits = cms.VInputTag('TCSimHits'),
    simClusters = cms.InputTag('overlapMerger', 'mergedSimClusters'),
    fcPerMip = fcPerMip,
    keV2fC = keV2fC,
    layerWeights = layercalibparam.triggerWeights.weights,
    thicknessCorrections = thicknessCorrections,
)

HGCModules = cms.EDProducer("L1THGCalModuleProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    tcTruth = cms.InputTag('L1HGCalTCsTruth'),
)

moduleTruth = cms.EDProducer("L1THGCalModuleTruthProducer",
    wafers = cms.InputTag('HGCModules'),
)

ECONdata = cms.EDProducer("L1THGCalECONdataProducer",
    wafers = cms.InputTag('HGCModules'),
    bitsPerADC = cms.uint32(22),  
    bitsPerNorm = cms.uint32(12),
    bitsPerCALQ = cms.uint32(23),
    bitsPerInput = cms.uint32(8),
    useModuleFactor = cms.bool(True),  
    bitShiftNormalize = cms.bool(True),
    useTransverseADC = cms.bool(True), 
    normByMax = cms.bool(True)
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

moduleTable = cms.EDProducer("SimpleHGCalModuleFlatTableProducer",
    src = cms.InputTag('HGCModules'),
    name = cms.string("L1THGCalModule"),
    singleton = cms.bool(False),  # the number of entries is variable
    extension = cms.bool(False),  # this is the main table for the modules
    variables = cms.PSet(
        moduleId = Var('moduleId()', 'int', doc='Module ID of the trigger cell'),
        energy = Var('energy()', 'float', doc='Energy of the module'),
        mipPt = Var('mipPt()', 'int', doc='MIP pT of the module'),
    )
)

triggerCellPropsTable = cms.EDProducer("L1HGCalTCPropertiesTableProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    name = cms.string("L1THGCalTC"),
)

modulePropsTable = cms.EDProducer("L1HGCalModulePropertiesTableProducer",
    triggerCells = cms.InputTag('HGCModules'),
    name = cms.string("L1THGCalModule"),
)

triggerCellTruthTable = cms.EDProducer("L1HGCalTCTruthTableProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    tcTruth = cms.InputTag('L1HGCalTCsTruth'),
    name = cms.string("L1THGCalTC"),
)

moduleTruthTable = cms.EDProducer("L1HGCalModuleTruthTableProducer",
    triggerCells = cms.InputTag('HGCModules'),
    tcTruth = cms.InputTag('moduleTruth'),
    name = cms.string("L1THGCalModule"),
)

ECONdataTable = cms.EDProducer("L1HGCalECONdataTableProducer",
    wafers=cms.InputTag('HGCModules'),
    ECONdata = cms.InputTag('ECONdata'),
    name = cms.string("L1THGCalModule"),
)

fullChain = cms.Sequence(
    TCSimHits *
    simTreeMerger *
    overlapMerger *
    SimonMergedSimClusterTable *
    SimClusterTCTable *
    L1HGCalTCsTruth *
    triggerCellTable *
    triggerCellPropsTable *
    triggerCellTruthTable *
    HGCModules *
    moduleTable *
    modulePropsTable *
    ECONdata *
    moduleTruth *
    moduleTruthTable *
    ECONdataTable
)
