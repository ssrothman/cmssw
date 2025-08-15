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
    distanceTol = cms.double(0.1),

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

#SimonMergedSimTrackTable = cms.EDProducer("SimpleSimTrackFlatTableProducer",
#    src = cms.InputTag('overlapMerger', 'mergedSimTracks'),
#    name = cms.string("SimonMergedSimTrack"),
#    extension = cms.bool(False),  # this is the main table for the simtracks
#    singleton = cms.bool(False),  # the number of entries is variable
#    variables = cms.PSet(
#
#        eta = Var('momentum().eta()', 'float', doc='eta of the simtrack'),
#        phi = Var('momentum().phi()', 'float', doc='phi of the simtrack'),
#        pt = Var('momentum().pt()', 'float', doc='pt of the simtrack'),
#        charge = Var('charge()', 'float', doc='charge of the simtrack'),
#        pdgId = Var('type()', 'int', doc='PDG ID of the simtrack'),
#    )
#)

L1HGCalTCsTruth = cms.EDProducer("L1TriggerCellTruthProducer",
    triggerCells = cms.InputTag('FloatingpointThreshold0', 'HGCalConcentratorProcessorSelection'),
    simHitsEE = cms.InputTag('g4SimHits','HGCHitsEE'),
    simHitsFH = cms.InputTag('g4SimHits','HGCHitsHEfront'),
    simHitsBH = cms.InputTag('g4SimHits','HGCHitsHEback'),
    simClusters = cms.InputTag('overlapMerger', 'mergedSimClusters'),
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

fullChain = cms.Sequence(
    TCSimHits *
    simTreeMerger *
    overlapMerger *
    SimonMergedSimClusterTable *
    #SimonMergedSimTrackTable *
    L1HGCalTCsTruth *
    triggerCellTable *
    triggerCellPropsTable *
    triggerCellTruthTable
)
