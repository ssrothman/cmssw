import FWCore.ParameterSet.Config as cms

model = 'dummy'
AEProducer = cms.EDProducer("ECONTritonProducer",
    Client = cms.PSet(
        mode = cms.string("Async"),
        modelName = cms.string(model),
        modelConfigPath = cms.FileInPath("L1Trigger/L1THGCal/data/models/%s/config.pbtxt"%model),
        allowedTries = cms.untracked.uint32(1),
        timeout = cms.untracked.uint32(10),
        useSharedMemory=cms.untracked.bool(False)
    ),
    TriggerCells = cms.InputTag("Floatingpoint","HGCalVFEProcessorSums"),
    inputType = cms.string("ADC"),
    normType = cms.string("None"),
    cut = cms.string("")
)
