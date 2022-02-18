import FWCore.ParameterSet.Config as cms

patElectronsDRN = cms.EDProducer("PatElectronDRNCorrectionProducer",
    particleSource = cms.InputTag("selectedPatElectrons"),
    rhoName = cms.InputTag("fixedGridRhoFastjetAll"),
    reducedEcalRecHitsEB = cms.InputTag("reducedEcalRecHitsEB"),
    reducedEcalRecHitsEE = cms.InputTag("reducedEcalRecHitsEE"),
    reducedEcalRecHitsES = cms.InputTag("reducedEcalRecHitsES"),

    Client = cms.PSet(
      mode = cms.string("Async"),
      modelName = cms.string("electronsEnsemble"),
      modelConfigPath = cms.FileInPath("RecoEgamma/EgammaPhotonProducers/data/models/electronsEnsemble/config.pbtxt"),
      allowedTries = cms.untracked.uint32(1),
      timeout = cms.untracked.uint32(10),
      useSharedMemory = cms.untracked.bool(False),
    )
)
