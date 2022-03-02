# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 -s PAT,VALIDATION:@miniAODValidation,DQM:@miniAODDQM --era Run2_2018 -n 100 --process PAT --conditions auto:phase1_2018_realistic --mc --scenario pp --eventcontent MINIAODSIM,DQM --datatier MINIAODSIM,DQMIO --procModifiers run2_miniAOD_UL_preSummer20 --no_exec --filein filelist:filelist --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018
from Configuration.ProcessModifiers.run2_miniAOD_UL_preSummer20_cff import run2_miniAOD_UL_preSummer20
from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton

process = cms.Process('PAT',Run2_2018,run2_miniAOD_UL_preSummer20, enableSonicTriton)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('Configuration.StandardSequences.PATMC_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('DQMServices.Core.DQMStoreNonLegacy_cff')
process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FFB42B2C-E0F9-B344-BAE4-C97165444822.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FCD6B136-75CB-9C43-A4B9-0668C14D219A.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E9A0886E-9F76-6148-A170-36A5DD266A3D.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/82B7766D-BFE2-2146-8F65-9C31BDBD4E3F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6BD4A402-3572-1A43-A906-D2E3FABFABCB.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1408CFD5-1436-1043-90E6-2BA7EF6B0D05.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2DF21EFD-6623-3943-8E50-EF08A6141612.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9EC674E1-7C9C-7D4A-A544-A0A0939E7943.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3392240A-3DEB-0645-8749-7740913DAD1C.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6538D06A-0378-4B4C-AA91-DCD9DFC81A29.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CBAEBFF7-6E48-B041-BB73-C75E6DA0D56F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/48305DEF-42D9-E749-922B-5B41892E74DF.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/07176D48-8D93-2843-AB98-10CF6584A369.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3234E70C-EA00-E444-89EE-611F7A212FDE.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0E6D5C96-0842-1144-BBBC-77B97D85E65E.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5F512C95-94E5-3642-A0ED-B911BF5E0D7A.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/98824C7F-20A4-D649-9EEA-8EBBA392403B.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/17462FE7-8232-CD41-8441-92388DDBF4A1.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B4DB5A00-21E4-284B-A4CF-485D31E6ACDE.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8D4D2CB1-346A-6C46-9021-C3A948983A21.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2015323B-AC60-744A-A747-4CEAD95BFC86.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E03525D2-FE5F-5544-BE81-48D5B916AB93.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D6BE7275-A85B-6048-A6C1-C8E70DD2490F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A9D05D7B-41E0-FD4D-BCE7-BF11FECE0666.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/65A3AAD3-4587-784E-91BF-79652F11F6E8.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B8F967F3-D110-0A43-B17A-5B3A0BAEDE78.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DCD291E8-9EFF-B247-B6AB-3A62C398E60C.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4B20DA9A-80FB-A74C-8C4D-DC89467AA7C0.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/20246A12-6013-FD40-B2D8-B635DED3DCA4.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2452FAEA-D20B-9848-9B46-CDEF40366AB0.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E0AFE16D-CC4E-3949-91DD-0DD8E85004BB.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/65D6B0CB-47CB-F84A-95E0-3F45AF32E19B.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D604927A-92DA-2C47-9F61-7BA7D167DC4A.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1B314E75-7B2A-8C4A-9841-A6BAAC8591D6.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F8F02CA5-0BC8-DD48-888B-E5E35089D305.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BCC3B2E7-2FDB-5248-861B-31B8274CE873.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/810E4F76-D053-F34E-84DD-216E923CE53A.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/689034DF-511A-B64E-8560-71658510D63B.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0187D20B-9C4B-B446-96F1-A8E2A7CD39EE.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5859D109-7940-0946-897C-C2CF6E2C0849.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/626A9C5D-1950-8042-AA9B-C9CE17E83A17.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BCCDB5F2-6134-1A4D-9E3F-98E1C110EF16.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D5D7C541-3020-F146-B888-6ECCD3F09639.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E2065954-67B9-E049-B61C-71914F461520.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E17D5ECD-6A45-D748-9364-003356FEFEE7.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/49158DD4-1981-C846-9270-88FFB677FEEB.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C3218B27-4921-7741-9357-0D0702E328FB.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/38BFDEB4-6128-944D-948B-94B5FE0FB3D1.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F25F02CE-3A45-AE47-9012-CFEBBC9B235F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/BD291243-6669-E64B-BB6C-8A63AB448899.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/26D026F8-39AB-8E4B-84A8-A166B164D956.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C8A941B7-9591-6E45-B076-90A5FE35450D.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/CF6B762F-1F18-6346-A89D-670D642C4638.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1CFC7C53-610A-C34B-A5F7-CBCE67942523.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/68A8D9E8-8782-434E-848F-F3DB5C161301.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6881E5D4-AFDF-2D48-BA98-BEF64B2CFC1F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9379818B-13DA-8446-AC7D-BC3AAEAAF043.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4CE40267-CCB4-3740-87D6-883ECAA921EC.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E034935D-0859-2A4F-ACF0-B8F1705240A0.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/7478DDA4-9A9D-554F-B819-BE819EC07983.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/2D633B6A-DE4E-9045-AB1F-3661247B6AB8.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5C862B18-6682-C946-8E54-3BCB165AB5AD.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B712EAE5-E542-E44D-AAEA-5621E7F6BC37.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3D26549D-0E50-D541-8F33-6C3E42D5975E.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/99FAEE9F-3400-FF42-9C19-29B2B8A02C86.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/51153708-45D4-7C4F-94B7-B4A229221A41.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5B66269B-DB1B-F94E-988E-70A90509A0B5.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/35910BB0-C132-ED48-9C6B-665A83FB7F7E.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8810934A-86A3-524E-8FF5-FB42FF5BDD0B.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3D078B33-BD6E-744C-B153-68B37872521E.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D4146AAD-9453-584E-A85F-79AC0B1D676F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/013D3114-DBD1-FC4B-BBD5-7E3D036A9790.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9F2CA230-F6C4-A942-9034-19DCD80EDF7D.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1C43D47D-814A-0C49-A5AC-7B2A5E05C4FB.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/622B730C-005B-164A-A7C6-9CDF871298BE.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1F4DCE46-AFED-1749-A281-93650612D9E7.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/95C752A4-9C39-AF46-8DC3-D07004387ED6.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/EC47FABE-8245-7C40-A828-58A63D68E5B4.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E85E0F42-4D02-8C45-9C00-5E30FA1C312F.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F45696F3-3C98-C947-B996-C94E45D201C0.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/09673D14-995E-6947-B3F6-6856AB0C12EC.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/17E126E6-1C3C-1B40-ADF9-87C0F45D6FBC.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4A107040-E585-9843-A970-DB49412EB891.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/43A338AA-384E-6D48-92D2-51D63FAD7E18.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1147F41F-EB0E-C641-9B3B-F4216708499D.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6DAB3AD7-9D6E-014B-918D-B5C33D106011.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/72AC4100-ADCF-5042-9093-CD4CF62CBA26.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/90E0ED99-1792-6A41-9C3E-C6AB1F076CD1.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A1984D22-C81B-E743-92E7-B7BCE3416481.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8AA1F7C2-E276-E849-A20D-00F6DDC97FFD.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5D048550-9FE8-734D-B9C3-94792F9857BE.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/AE58109E-B54A-EC43-A268-B8CA518B1B2A.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6B1D429A-EE9B-7945-BE39-A4145B6D38E7.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C21CA046-92D4-3042-B5F8-9190C35924EA.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E6EAADE3-0497-0745-83C9-572E2A507188.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C34C4F9A-E8B2-B547-9D3A-9FEC7AC577DA.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/66A4D0D9-6D06-2541-9712-17CD19F7573E.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B3357BBF-0AA5-824D-AB53-CB6C88258C98.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/14589FF5-C583-1741-8D58-34DBEA27DAF2.root',
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D891CEEF-A99E-B345-80BA-D0FB99D8444E.root'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAODSIM'),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(-900),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('file:miniAOD_DRN_ttH.root'),
    outputCommands = process.MINIAODSIMEventContent.outputCommands,
    overrideBranchesSplitLevel = cms.untracked.VPSet(
        cms.untracked.PSet(
            branch = cms.untracked.string('patPackedCandidates_packedPFCandidates__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('recoGenParticles_prunedGenParticles__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('patTriggerObjectStandAlones_slimmedPatTrigger__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('patPackedGenParticles_packedGenParticles__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('patJets_slimmedJets__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('recoVertexs_offlineSlimmedPrimaryVertices__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('recoVertexs_offlineSlimmedPrimaryVerticesWithBS__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('recoCaloClusters_reducedEgamma_reducedESClusters_*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('EcalRecHitsSorted_reducedEgamma_reducedEBRecHits_*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('EcalRecHitsSorted_reducedEgamma_reducedEERecHits_*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('recoGenJets_slimmedGenJets__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('patJets_slimmedJetsPuppi__*'),
            splitLevel = cms.untracked.int32(99)
        ),
        cms.untracked.PSet(
            branch = cms.untracked.string('EcalRecHitsSorted_reducedEgamma_reducedESRecHits_*'),
            splitLevel = cms.untracked.int32(99)
        )
    ),
    overrideInputFileSplitLevels = cms.untracked.bool(True),
    splitLevel = cms.untracked.int32(0)
)

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2_inDQM.root'),
    outputCommands = process.DQMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition



# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

# Path and EndPath definitions
process.Flag_BadChargedCandidateFilter = cms.Path(process.BadChargedCandidateFilter)
process.Flag_BadChargedCandidateSummer16Filter = cms.Path(process.BadChargedCandidateSummer16Filter)
process.Flag_BadPFMuonDzFilter = cms.Path(process.BadPFMuonDzFilter)
process.Flag_BadPFMuonFilter = cms.Path(process.BadPFMuonFilter)
process.Flag_BadPFMuonSummer16Filter = cms.Path(process.BadPFMuonSummer16Filter)
process.Flag_CSCTightHalo2015Filter = cms.Path(process.CSCTightHalo2015Filter)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_CSCTightHaloTrkMuUnvetoFilter = cms.Path(process.CSCTightHaloTrkMuUnvetoFilter)
process.Flag_EcalDeadCellBoundaryEnergyFilter = cms.Path(process.EcalDeadCellBoundaryEnergyFilter)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_HBHENoiseFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseFilter)
process.Flag_HBHENoiseIsoFilter = cms.Path(process.HBHENoiseFilterResultProducer+process.HBHENoiseIsoFilter)
process.Flag_HcalStripHaloFilter = cms.Path(process.HcalStripHaloFilter)
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_chargedHadronTrackResolutionFilter = cms.Path(process.chargedHadronTrackResolutionFilter)
process.Flag_ecalBadCalibFilter = cms.Path(process.ecalBadCalibFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_eeBadScFilter = cms.Path(process.eeBadScFilter)
process.Flag_globalSuperTightHalo2016Filter = cms.Path(process.globalSuperTightHalo2016Filter)
process.Flag_globalTightHalo2016Filter = cms.Path(process.globalTightHalo2016Filter)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)
process.Flag_hfNoisyHitsFilter = cms.Path(process.hfNoisyHitsFilter)
process.Flag_muonBadTrackFilter = cms.Path(process.muonBadTrackFilter)
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_trkPOGFilters = cms.Path(process.trkPOGFilters)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_trkPOG_manystripclus53X = cms.Path(~process.manystripclus53X)
process.Flag_trkPOG_toomanystripclus53X = cms.Path(~process.toomanystripclus53X)
process.prevalidation_step = cms.Path(process.prevalidationMiniAOD)
process.validation_step = cms.EndPath(process.validationMiniAOD)
process.dqmoffline_step = cms.EndPath(process.DQMOfflineMiniAOD)
process.dqmofflineOnPAT_step = cms.EndPath(process.PostDQMOfflineMiniAOD)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.Flag_HBHENoiseFilter,process.Flag_HBHENoiseIsoFilter,process.Flag_CSCTightHaloFilter,process.Flag_CSCTightHaloTrkMuUnvetoFilter,process.Flag_CSCTightHalo2015Filter,process.Flag_globalTightHalo2016Filter,process.Flag_globalSuperTightHalo2016Filter,process.Flag_HcalStripHaloFilter,process.Flag_hcalLaserEventFilter,process.Flag_EcalDeadCellTriggerPrimitiveFilter,process.Flag_EcalDeadCellBoundaryEnergyFilter,process.Flag_ecalBadCalibFilter,process.Flag_goodVertices,process.Flag_eeBadScFilter,process.Flag_ecalLaserCorrFilter,process.Flag_trkPOGFilters,process.Flag_chargedHadronTrackResolutionFilter,process.Flag_muonBadTrackFilter,process.Flag_BadChargedCandidateFilter,process.Flag_BadPFMuonFilter,process.Flag_BadPFMuonDzFilter,process.Flag_hfNoisyHitsFilter,process.Flag_BadChargedCandidateSummer16Filter,process.Flag_BadPFMuonSummer16Filter,process.Flag_trkPOG_manystripclus53X,process.Flag_trkPOG_toomanystripclus53X,process.Flag_trkPOG_logErrorTooManyClusters,process.Flag_METFilters,process.prevalidation_step,process.validation_step,process.dqmoffline_step,process.dqmofflineOnPAT_step,process.MINIAODSIMoutput_step,process.DQMoutput_step)
process.schedule.associate(process.patTask)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 

#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllMC(process)

from Validation.Performance.TimeMemoryInfo import customise as customise_profile

process = customise_profile(process)

from PhysicsTools.PatAlgos.slimming.enableDRN import enableDRN

process = enableDRN(process)

process.TritonService.verbose = cms.untracked.bool(True)
process.TritonService.fallback.verbose = cms.untracked.bool(True)

keepMsgs = ['TritonClient', 'TritonService', 'DRNCorrectionProducerT', 'DRNCorrectionProducerT:TritonClient']
for msg in keepMsgs:
  setattr(process.MessageLogger.cerr, msg,
      cms.untracked.PSet(
        limit = cms.untracked.int32(100000000),
      )
  )

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
from Configuration.ProcessModifiers.enableSonicTriton_cff import enableSonicTriton

# End adding early deletion

