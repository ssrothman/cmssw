from FWCore.ParameterSet.VarParsing import VarParsing
import FWCore.ParameterSet.Config as cms
import os, sys, json

# module/model correspondence
models = {
    "TritonImageProducer": ["inception_graphdef", "densenet_onnx"],
    "TritonGraphProducer": ["gat_test"],
    "TritonGraphFilter": ["gat_test"],
    "TritonGraphAnalyzer": ["gat_test"],
    "DRNProducer" : ["EGM_DRN"] 
}

# other choices
allowed_modes = ["Async","PseudoAsync","Sync"]
allowed_compression = ["none","deflate","gzip"]
allowed_devices = ["auto","cpu","gpu"]

options = VarParsing()
options.register("maxEvents", -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "Number of events to process (-1 for all)")
options.register("serverName", "default", VarParsing.multiplicity.singleton, VarParsing.varType.string, "name for server (used internally)")
options.register("address", "", VarParsing.multiplicity.singleton, VarParsing.varType.string, "server address")
options.register("port", 8001, VarParsing.multiplicity.singleton, VarParsing.varType.int, "server port")
options.register("timeout", 30, VarParsing.multiplicity.singleton, VarParsing.varType.int, "timeout for requests")
options.register("params", "", VarParsing.multiplicity.singleton, VarParsing.varType.string, "json file containing server address/port")
options.register("threads", 1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "number of threads")
options.register("streams", 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "number of streams")
options.register("modules", "TritonGraphProducer", VarParsing.multiplicity.list, VarParsing.varType.string, "list of modules to run (choices: {})".format(', '.join(models)))
options.register("models","gat_test", VarParsing.multiplicity.list, VarParsing.varType.string, "list of models (same length as modules, or just 1 entry if all modules use same model)")
options.register("mode","Async", VarParsing.multiplicity.singleton, VarParsing.varType.string, "mode for client (choices: {})".format(', '.join(allowed_modes)))
options.register("verbose", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "enable verbose output")
options.register("brief", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "briefer output for graph modules")
options.register("unittest", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "unit test mode: reduce input sizes")
options.register("testother", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "also test gRPC communication if shared memory enabled, or vice versa")
options.register("shm", True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "enable shared memory")
options.register("compression", "", VarParsing.multiplicity.singleton, VarParsing.varType.string, "enable I/O compression (choices: {})".format(', '.join(allowed_compression)))
options.register("ssl", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "enable SSL authentication for server communication")
options.register("device","auto", VarParsing.multiplicity.singleton, VarParsing.varType.string, "specify device for fallback server (choices: {})".format(', '.join(allowed_devices)))
options.register("docker", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "use Docker for fallback server")
options.register("tries", 0, VarParsing.multiplicity.singleton, VarParsing.varType.int, "number of retries for failed request")
options.register("batchSize", 1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "batchSize")
options.parseArguments()

if len(options.params)>0:
    with open(options.params,'r') as pfile:
        pdict = json.load(pfile)
    options.address = pdict["address"]
    options.port = int(pdict["port"])
    print("server = "+options.address+":"+str(options.port))

# check models and modules
if len(options.modules)!=len(options.models):
    # assigning to VarParsing.multiplicity.list actually appends to existing value(s)
    if len(options.models)==1: options.models = [options.models[0]]*(len(options.modules)-1)
    else: raise ValueError("Arguments for modules and models must have same length")
for im,module in enumerate(options.modules):
    if module not in models:
        raise ValueError("Unknown module: {}".format(module))
    model = options.models[im]
    if model not in models[module]:
        raise ValueError("Unsupported model {} for module {}".format(model,module))

# check modes
if options.mode not in allowed_modes:
    raise ValueError("Unknown mode: {}".format(options.mode))

# check compression
if len(options.compression)>0 and options.compression not in allowed_compression:
    raise ValueError("Unknown compression setting: {}".format(options.compression))

# check devices
options.device = options.device.lower()
if options.device not in allowed_devices:
	raise ValueError("Unknown device: {}".format(options.device))

from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton
process = cms.Process('tritonTest',enableSonicTriton)

process.load("HeterogeneousCore.SonicTriton.TritonService_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://cms-xrd-global.cern.ch///store/mc/RunIISummer20UL18RECO/DoubleElectron_Pt-1To300-gun/AODSIM/FlatPU0to70EdalIdealGT_EdalIdealGT_106X_upgrade2018_realistic_v11_L1v1_EcalIdealIC-v2/270000/4CDD9457-E14C-D84A-9BD4-3140CB6AEEB6.root'#, \
    )
)

process.TritonService.verbose = options.verbose
process.TritonService.fallback.verbose = options.verbose
process.TritonService.fallback.useDocker = options.docker
if options.device != "auto":
    process.TritonService.fallback.useGPU = options.device=="gpu"
if len(options.address)>0:
    process.TritonService.servers.append(
        cms.PSet(
            name = cms.untracked.string(options.serverName),
            address = cms.untracked.string(options.address),
            port = cms.untracked.uint32(options.port),
            useSsl = cms.untracked.bool(options.ssl),
            rootCertificates = cms.untracked.string(""),
            privateKey = cms.untracked.string(""),
            certificateChain = cms.untracked.string(""),
        )
    )

# Let it run
modules = {
    "Producer": cms.EDProducer,
    "Filter": cms.EDFilter,
    "Analyzer": cms.EDAnalyzer,
}

keepMsgs = ['TritonClient','TritonService']

process.DRNProducerEB = cms.EDProducer('DRNProducer',
    Client = cms.PSet(
        mode = cms.string(options.mode),
        preferredServer = cms.untracked.string(""),
        timeout = cms.untracked.uint32(options.timeout),
        modelName = cms.string(model),
        modelVersion = cms.string(""),
        modelConfigPath = cms.FileInPath("Progression/EGM_DRN/data/models/{}/config.pbtxt".format(model)),
        verbose = cms.untracked.bool(options.verbose),
        allowedTries = cms.untracked.uint32(options.tries),
        useSharedMemory = cms.untracked.bool(options.shm),
        compression = cms.untracked.string(options.compression),
    ),
    nodeMin = cms.uint32(1),
    nodeMax = cms.uint32(200),
    brief = cms.bool(options.brief),

    batchSize = cms.uint32(options.batchSize),

    inputSCs = cms.InputTag('particleFlowSuperClusterECAL','particleFlowSuperClusterECALBarrel'),
)

process.DRNProducerEE = cms.EDProducer('DRNProducer',
    Client = cms.PSet(
        mode = cms.string(options.mode),
        preferredServer = cms.untracked.string(""),
        timeout = cms.untracked.uint32(options.timeout),
        modelName = cms.string(model),
        modelVersion = cms.string(""),
        modelConfigPath = cms.FileInPath("Progression/EGM_DRN/data/models/{}/config.pbtxt".format(model)),
        verbose = cms.untracked.bool(options.verbose),
        allowedTries = cms.untracked.uint32(options.tries),
        useSharedMemory = cms.untracked.bool(options.shm),
        compression = cms.untracked.string(options.compression),
    ),
    nodeMin = cms.uint32(1),
    nodeMax = cms.uint32(200),
    brief = cms.bool(options.brief),

    batchSize = cms.uint32(options.batchSize),

    inputSCs = cms.InputTag('particleFlowSuperClusterECAL','particleFlowSuperClusterECALEndcapWithPreshower'),
)


#process.p += process.DRNProducer
keepMsgs.extend(['DRNProducer','DRNProducer:TritonClient'])

from Configuration.AlCa.GlobalTag import GlobalTag
from CondCore.DBCommon.CondDBSetup_cfi import *
process.GlobalTag = cms.ESSource("PoolDBESSource",
                               CondDBSetup,
                               connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
                               globaltag = cms.string('106X_upgrade2018_realistic_v11_Ecal4'),
			       toGet = cms.VPSet(


                cms.PSet(record = cms.string("GBRDWrapperRcd"),
                        tag = cms.string("DoubleElectron_FlatPt-1To300_FlatPU0to70_ECAL5_106X_upgrade2018_realistic_v11_L1v1-v2"),
                        label = cms.untracked.string("pfscecal_EBCorrection_offline_v2"),
                        connect = cms.string("sqlite_file:DBFiles/correctedECALSampleDBFile_EB.db")
                        ),

                cms.PSet(record = cms.string("GBRDWrapperRcd"),
                        tag = cms.string("DoubleElectron_FlatPt-1To300_FlatPU0to70_ECAL5_106X_upgrade2018_realistic_v11_L1v1-v2"),
                        label = cms.untracked.string("pfscecal_EBUncertainty_offline_v2"),
                        connect = cms.string("sqlite_file:DBFiles/correctedECALSampleDBFile_EB.db")
                        ),


                cms.PSet(record = cms.string("GBRDWrapperRcd"),
                        tag = cms.string("DoubleElectron_FlatPt-1To300_FlatPU0to70_ECAL5_106X_upgrade2018_realistic_v11_L1v1-v2"),
                        label = cms.untracked.string("pfscecal_EECorrection_offline_v2"),
                        connect = cms.string("sqlite_file:DBFiles/correctedECALSampleDBFile_EE.db")
                        ),


                cms.PSet(record = cms.string("GBRDWrapperRcd"),
                        tag = cms.string("DoubleElectron_FlatPt-1To300_FlatPU0to70_ECAL5_106X_upgrade2018_realistic_v11_L1v1-v2"),
                        label = cms.untracked.string("pfscecal_EEUncertainty_offline_v2"),
                        connect = cms.string("sqlite_file:DBFiles/correctedECALSampleDBFile_EE.db")
                        )



                        )

)

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)

my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']

for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)


process.nTuplelize = cms.EDAnalyzer('Electron_RecHit_NTuplizer',
        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
        rhoFastJet = cms.InputTag("fixedGridRhoFastjetAll"),
        pileupInfo = cms.InputTag("addPileupInfo"),
        electrons = cms.InputTag("gedGsfElectrons"),
        genParticles = cms.InputTag("genParticles"),
        #Cut Based Id
        eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-loose"),
        eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-medium"),
        eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V2-tight")
        )


process.TFileService = cms.Service("TFileService",
     fileName = cms.string("nTupleMC.root"),
      closeFileFast = cms.untracked.bool(True)
  )

#process+=process.nTuplelize
process.p = cms.Path(process.DRNProducerEB*process.DRNProducerEE*process.egmGsfElectronIDSequence*process.nTuplelize)



'''
for im,module in enumerate(options.modules):
    model = options.models[im]
    Module = [obj for name,obj in modules.items() if name in module][0]
    setattr(process, module,
        Module(module,
            Client = cms.PSet(
                mode = cms.string(options.mode),
                preferredServer = cms.untracked.string(""),
                timeout = cms.untracked.uint32(options.timeout),
                modelName = cms.string(model),
                modelVersion = cms.string(""),
                modelConfigPath = cms.FileInPath("Progression/EGM_DRN/data/models/{}/config.pbtxt".format(model)),
                verbose = cms.untracked.bool(options.verbose),
                allowedTries = cms.untracked.uint32(options.tries),
                useSharedMemory = cms.untracked.bool(options.shm),
                compression = cms.untracked.string(options.compression),
            )
        )
    )
    processModule = getattr(process, module)
    processModule.nodeMin = cms.uint32(1)
    processModule.nodeMax = cms.uint32(200)
    processModule.brief = cms.bool(options.brief)
    process.p += processModule
    keepMsgs.extend([module,module+':TritonClient'])
    if options.testother:
        # clone modules to test both gRPC and shared memory
        _module2 = module+"GRPC" if processModule.Client.useSharedMemory else "SHM"
        setattr(process, _module2,
            processModule.clone(
                Client = dict(useSharedMemory = not processModule.Client.useSharedMemory)
            )
        )
        processModule2 = getattr(process, _module2)
        process.p += processModule2
'''
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 500
for msg in keepMsgs:
    setattr(process.MessageLogger.cerr,msg,
        cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
        )
    )

if options.threads>0:
    process.options.numberOfThreads = options.threads
    process.options.numberOfStreams = options.streams

