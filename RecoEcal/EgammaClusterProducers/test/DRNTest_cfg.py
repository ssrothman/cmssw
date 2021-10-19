import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton
process = cms.Process("Demo",enableSonicTriton)

process.load("HeterogeneousCore.SonicTriton.TritonService_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
#process.options.numberOfThreads = cms.untracked.uint32(4)
#process.options.numberOfStreams = cms.untracked.uint32(8)
process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')

process.TritonService.verbose = False
process.TritonService.fallback.verbose = False
process.TritonService.fallback.useDocker = False
process.TritonService.fallback.useGPU = False

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
'root://cms-xrd-global.cern.ch///store/mc/RunIISummer20UL18RECO/DoubleElectron_Pt-1To300-gun/AODSIM/FlatPU0to70EdalIdealGT_EdalIdealGT_106X_upgrade2018_realistic_v11_L1v1_EcalIdealIC-v2/270000/4CDD9457-E14C-D84A-9BD4-3140CB6AEEB6.root'
                )
                            )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2018_realistic_v11_Ecal5', '')
s
from RecoEcal.EgammaClusterProducers.SCEnergyCorrectorDRNProducer_cfig import *

process.DRNProducerEB = DRNProducerEB
process.DRNProducerEE = DRNProducerEE

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']

for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)

process.nTuplelize = cms.EDAnalyzer('DRNTestNtuplizer',
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        rhoFastJet = cms.InputTag("fixedGridRhoFastjetAll"),
        pileupInfo = cms.InputTag("addPileupInfo"),
        electrons = cms.InputTag("gedGsfElectrons"),
        genParticles = cms.InputTag("genParticles"),
        #Cut Based Id
        eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-loose"),
        eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-medium"),
        eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-tight"),

        tracks = cms.InputTag("globalTracks"),
        SkipEvent = cms.untracked.vstring('ProductNotFound')
        )

process.TFileService = cms.Service("TFileService",
     fileName = cms.string("nTupleMC.root"),
      closeFileFast = cms.untracked.bool(True)
  )

process.load( "HLTrigger.Timer.FastTimerService_cfi" )

'''
process.FastTimerService = cms.Service( "FastTimerService",
    dqmPath = cms.untracked.string( "timer" ),
    dqmModuleTimeRange = cms.untracked.double( 40.0 ),
    enableDQMbyPath = cms.untracked.bool( True ),
    writeJSONSummary = cms.untracked.bool( True ),
    dqmPathMemoryResolution = cms.untracked.double( 5000.0 ),
    enableDQM = cms.untracked.bool( True ),
    enableDQMbyModule = cms.untracked.bool( True ),
    dqmModuleMemoryRange = cms.untracked.double( 100000.0 ),
    dqmModuleMemoryResolution = cms.untracked.double( 500.0 ),
    dqmMemoryResolution = cms.untracked.double( 5000.0 ),
    enableDQMbyLumiSection = cms.untracked.bool( True ),
    dqmPathTimeResolution = cms.untracked.double( 0.5 ),
    printEventSummary = cms.untracked.bool( False ),
    dqmPathTimeRange = cms.untracked.double( 100.0 ),
    dqmTimeRange = cms.untracked.double( 2000.0 ),
    enableDQMTransitions = cms.untracked.bool( False ),
    dqmPathMemoryRange = cms.untracked.double( 1000000.0 ),
    dqmLumiSectionsRange = cms.untracked.uint32( 2500 ),
    enableDQMbyProcesses = cms.untracked.bool( True ),
    dqmMemoryRange = cms.untracked.double( 1000000.0 ),
    dqmTimeResolution = cms.untracked.double( 5.0 ),
    printRunSummary = cms.untracked.bool( False ),
    dqmModuleTimeResolution = cms.untracked.double( 0.2 ),
    printJobSummary = cms.untracked.bool( True ),
)
'''

'''
process.ThroughputService = cms.Service( "ThroughputService",
    dqmPath = cms.untracked.string( "throughput" ),
    eventRange = cms.untracked.uint32( 10000 ),
    timeRange = cms.untracked.double( 60000.0 ),
    printEventSummary = cms.untracked.bool( True ),
    eventResolution = cms.untracked.uint32( 100 ),
    enableDQM = cms.untracked.bool( True ),
    dqmPathByProcesses = cms.untracked.bool( True ),
    timeResolution = cms.untracked.double( 5.828 )
)
'''

'''
process.dqmOutput = cms.OutputModule("DQMRootOutputModule",
    fileName = cms.untracked.string("DQM.root")
)
'''

#process.p = cms.Path(process.DRNProducerEB*process.egmGsfElectronIDSequence*process.nTuplelize)

process.p = cms.Path(process.DRNProducerEB*process.DRNProducerEE*process.egmGsfElectronIDSequence*process.nTuplelize)
#process.p = cms.Path(process.DRNProducerEB*process.DRNProducerEE)
