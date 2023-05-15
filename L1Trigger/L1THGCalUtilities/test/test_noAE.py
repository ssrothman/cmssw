import FWCore.ParameterSet.Config as cms 

from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton
from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9
process = cms.Process('DIGI',Phase2C17I13M9)

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
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:DoubleElectron_FlatPt-1To100-gun_noPU.root'),
    #fileNames = cms.untracked.vstring('/store/mc/Phase2Fall22DRMiniAOD/DoubleElectron_FlatPt-1To100-gun/GEN-SIM-DIGI-RAW-MINIAOD/noPU_125X_mcRun4_realistic_v2-v1/2550000/066944a3-a061-42ac-ba45-9faadb46407a.root'),
    fileNames = cms.untracked.vstring(
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/0ded0ca9-3a66-405d-bded-83d303a9c799.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/0df00bcc-d3d6-4502-b087-5bf8ad5e5f40.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/0df93a72-3417-467c-b72e-2d4b9e862de5.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/105accd4-e83c-40ae-814c-c2b4735c791f.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/10a2eff8-1514-4273-94c4-448432c8c273.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/11f8cfd2-e5b8-4ced-8d7d-a3d7bd72d151.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/129b02c2-7ba6-459d-80ea-d460ec9b61a1.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/12ca67ca-7592-4d25-9f00-20260e10bcf2.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/12ea965c-8ac8-44f6-889c-7b04b0080deb.root',
        'file:/home/submit/srothman/cmsdata/ECON_datasets/DoubleElectron_FlatPt-1To100_PU200/MINIAOD/14035d77-8ae3-448b-bea4-8614189e6b30.root',
),

    inputCommands=cms.untracked.vstring(
        'keep *',
        'drop l1tTkPrimaryVertexs_L1TkPrimaryVertex__RECO',
    )
)

process.options = cms.untracked.PSet(

)
process.options.numberOfThreads = cms.untracked.uint32(1)
process.options.numberOfStreams = cms.untracked.uint32(1)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleElectronPt10_cfi nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition
process.TFileService = cms.Service(
    "TFileService",
    #fileName = cms.string("/home/submit/srothman/cmsdata/hgcal/myntuples/ntuple_noAE6.root")
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
chains.register_concentrator("Threshold0", concentrator.CreateThreshold(
  threshold_scintillator=cms.double(-1),
  threshold_silicon=cms.double(-1)
))
chains.register_concentrator("NominalTot", concentrator.CreateAutoencoder(
  useTransverseADC=False,
  skipAE=True,
  useModuleFactor=False,
  bitShiftNormalization=True,
  normByMax=False,
))
chains.register_concentrator("NormByMaxTot", concentrator.CreateAutoencoder(
  useTransverseADC=False,
  skipAE=True,
  useModuleFactor=False,
  bitShiftNormalization=True,
  normByMax=True,
))
chains.register_concentrator("Nominal", concentrator.CreateAutoencoder(
  useTransverseADC=True,
  skipAE=True,
  useModuleFactor=False,
  bitShiftNormalization=True,
  normByMax=False,
))
chains.register_concentrator("NormByMax", concentrator.CreateAutoencoder(
    useTransverseADC=True,
    skipAE=True,
    useModuleFactor=False,
    bitShiftNormalization=True,
    normByMax=True,
))
chains.register_concentrator("NormByMaxModuleFactor", concentrator.CreateAutoencoder(
    useTransverseADC=True,
    skipAE=True,
    useModuleFactor=True,
    bitShiftNormalization=True,
    normByMax=True,
))
chains.register_concentrator("ModuleFactor", concentrator.CreateAutoencoder(
    useTransverseADC=True,
    skipAE=True,
    useModuleFactor=True,
    bitShiftNormalization=True,
    normByMax=False,
))

## BE1
chains.register_backend1("Dummy", clustering2d.CreateDummy())
## BE2
chains.register_backend2("Histomax", clustering3d.CreateHistoMax())
# Register selector
chains.register_selector("Dummy", selectors.CreateDummy())


# Register ntuples
ntuple_list = ['event', 'gen', 'multiclusters', 'triggercells']
chains.register_ntuple("nTuple", ntuple.CreateNtuple(ntuple_list))

# Register trigger chains
concentrator_algos = ['Threshold0', "Nominal", "NormByMax", "NormByMaxModuleFactor", "ModuleFactor", "NominalTot", 'NormByMaxTot']

backend1_algos = ['Dummy']# 'Distance'] #'Topological']#, 'ConstrTopological']
backend2_algos = ['Histomax']#, 'Distance', 'Dbscan']
## Make cross product fo ECON and BE algos
import itertools
for cc,be1,be2 in itertools.product(concentrator_algos,backend1_algos, backend2_algos):
    chains.register_chain('Floatingpoint', cc, be1, be2, 'Dummy', 'nTuple')

process = chains.create_sequences(process)

# Remove towers from sequence
process.L1THGCalTriggerPrimitives.remove(process.L1THGCalTowerMap)
process.L1THGCalTriggerPrimitives.remove(process.L1THGCalTower)

from CommonTools.CandAlgos.genParticleCustomSelector_cfi import genParticleCustomSelector
process.filter = genParticleCustomSelector.clone(
    minRapidity = -1.444,
    maxRapidity = 1.444,
    invertRapidityCut = True,
    filter = cms.bool(True)
)

process.hgcl1tpg_step = cms.Path(process.L1THGCalTriggerPrimitives)
process.selector_step = cms.Path(process.L1THGCalTriggerSelector)
process.ntuple_step = cms.Path(process.L1THGCalTriggerNtuples)

process.fullpath = cms.Path(process.filter + process.L1THGCalTriggerPrimitives + process.L1THGCalTriggerSelector + process.L1THGCalTriggerNtuples)

# Schedule definition
process.schedule = cms.Schedule(process.fullpath)
#process.schedule = cms.Schedule(process.hgcl1tpg_step, process.selector_step, process.ntuple_step)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

