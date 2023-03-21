import FWCore.ParameterSet.Config as cms 

from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton
from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9
process = cms.Process('DIGI',Phase2C17I13M9, enableSonicTriton)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D88Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D88_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC14TeV_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:DoubleElectron_FlatPt-1To100-gun_noPU.root'),
    inputCommands=cms.untracked.vstring(
        'keep *',
        'drop l1tTkPrimaryVertexs_L1TkPrimaryVertex__RECO',
    )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleElectronPt10_cfi nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("ntuple.root")
    )

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T21', '')

# load HGCAL TPG simulation
process.load('L1Trigger.L1THGCal.hgcalTriggerPrimitives_cff')
process.load('L1Trigger.L1THGCalUtilities.HGC3DClusterSelectors_cff')
process.load('L1Trigger.L1THGCalUtilities.hgcalTriggerNtuples_cff')
from L1Trigger.L1THGCalUtilities.hgcalTriggerChains import HGCalTriggerChains
import L1Trigger.L1THGCalUtilities.vfe as vfe
import L1Trigger.L1THGCalUtilities.concentrator as concentrator
import L1Trigger.L1THGCalUtilities.clustering2d as clustering2d
import L1Trigger.L1THGCalUtilities.clustering3d as clustering3d
import L1Trigger.L1THGCalUtilities.selectors as selectors
import L1Trigger.L1THGCalUtilities.customNtuples as ntuple
process.ntuple_triggercells.FillSimEnergy=True


chains = HGCalTriggerChains()
# Register algorithms
## VFE
chains.register_vfe("Floatingpoint", vfe.CreateVfe())
## ECON
chains.register_concentrator("Supertriggercell", concentrator.CreateSuperTriggerCell())
chains.register_concentrator("Threshold0", concentrator.CreateThreshold(
  threshold_scintillator=cms.double(-1),
  threshold_silicon=cms.double(-1)
))
chains.register_concentrator("Bestchoice", concentrator.CreateBestChoice())
chains.register_concentrator("AutoEncoder", concentrator.CreateAutoencoder())
chains.register_concentrator("DummyAE", concentrator.CreateTritonAE(
  inputType = "ADCT",
  modelName = 'dummy',
  AEProducerName = 'AEProducerDummy',
  normType='None',
))
chains.register_concentrator("NateAE", concentrator.CreateTritonAE(
  inputType = "ADCT",
  modelName = "model_48_250_100_16_ensemble",
  AEProducerName = "AEProducerNate",
  normType='None',
))
chains.register_concentrator("NateAENorm", concentrator.CreateTritonAE(
  inputType = "ADCT",
  modelName = "model_48_250_100_16_ensemble",
  AEProducerName = "AEProducerNateFloating",
  normType='Floating',
))
chains.register_concentrator("RohanAE", concentrator.CreateAutoencoder(
  threshold_scintillator=-1,
  threshold_silicon=-1,
  zeroSuppresionThreshold=-1,
  saveEncodedValues=True,
  preserveModuleSum=True
))
chains.register_concentrator("RohanAEFP", concentrator.CreateAutoencoder(
  threshold_scintillator=-1,
  threshold_silicon=-1,
  zeroSuppresionThreshold=-1,
  saveEncodedValues=True,
  preserveModuleSum=True,
  nBitsPerInput=-1,
  maxBitsPerOutput=-1,
  clipTransverseADC=False,
))

## BE1
chains.register_backend1("Dummy", clustering2d.CreateDummy())
## BE2
chains.register_backend2("Histomax", clustering3d.CreateHistoMax())
# Register selector
chains.register_selector("Dummy", selectors.CreateDummy())


# Register ntuples
ntuple_list = ['event', 'gen', 'multiclusters', 'triggercells', 'econdata']
chains.register_ntuple("nTuple", ntuple.CreateNtuple(ntuple_list))

# Register trigger chains
#concentrator_algos = ['Supertriggercell', 'Threshold', 'Bestchoice', 'AutoEncoder', "TritonAE"]
concentrator_algos = ['Threshold0', "RohanAE", "RohanAEFP", "NateAE", "NateAENorm", "DummyAE"]

backend_algos = ['Histomax']
## Make cross product fo ECON and BE algos
import itertools
for cc,be in itertools.product(concentrator_algos,backend_algos):
    chains.register_chain('Floatingpoint', cc, 'Dummy', be, 'Dummy', 'nTuple')

process = chains.create_sequences(process)

# Remove towers from sequence
process.L1THGCalTriggerPrimitives.remove(process.L1THGCalTowerMap)
process.L1THGCalTriggerPrimitives.remove(process.L1THGCalTower)

process.hgcl1tpg_step = cms.Path(process.L1THGCalTriggerPrimitives)
process.selector_step = cms.Path(process.L1THGCalTriggerSelector)
process.ntuple_step = cms.Path(process.L1THGCalTriggerNtuples)

# Schedule definition
process.schedule = cms.Schedule(process.hgcl1tpg_step, process.selector_step, process.ntuple_step)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

