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
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FFB42B2C-E0F9-B344-BAE4-C97165444822.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FCD6B136-75CB-9C43-A4B9-0668C14D219A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E9A0886E-9F76-6148-A170-36A5DD266A3D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/82B7766D-BFE2-2146-8F65-9C31BDBD4E3F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6BD4A402-3572-1A43-A906-D2E3FABFABCB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1408CFD5-1436-1043-90E6-2BA7EF6B0D05.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2DF21EFD-6623-3943-8E50-EF08A6141612.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9EC674E1-7C9C-7D4A-A544-A0A0939E7943.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3392240A-3DEB-0645-8749-7740913DAD1C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6538D06A-0378-4B4C-AA91-DCD9DFC81A29.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CBAEBFF7-6E48-B041-BB73-C75E6DA0D56F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/48305DEF-42D9-E749-922B-5B41892E74DF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/07176D48-8D93-2843-AB98-10CF6584A369.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3234E70C-EA00-E444-89EE-611F7A212FDE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0E6D5C96-0842-1144-BBBC-77B97D85E65E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5F512C95-94E5-3642-A0ED-B911BF5E0D7A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/98824C7F-20A4-D649-9EEA-8EBBA392403B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/17462FE7-8232-CD41-8441-92388DDBF4A1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B4DB5A00-21E4-284B-A4CF-485D31E6ACDE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8D4D2CB1-346A-6C46-9021-C3A948983A21.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2015323B-AC60-744A-A747-4CEAD95BFC86.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E03525D2-FE5F-5544-BE81-48D5B916AB93.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D6BE7275-A85B-6048-A6C1-C8E70DD2490F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A9D05D7B-41E0-FD4D-BCE7-BF11FECE0666.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/65A3AAD3-4587-784E-91BF-79652F11F6E8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B8F967F3-D110-0A43-B17A-5B3A0BAEDE78.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DCD291E8-9EFF-B247-B6AB-3A62C398E60C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4B20DA9A-80FB-A74C-8C4D-DC89467AA7C0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/20246A12-6013-FD40-B2D8-B635DED3DCA4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2452FAEA-D20B-9848-9B46-CDEF40366AB0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E0AFE16D-CC4E-3949-91DD-0DD8E85004BB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/65D6B0CB-47CB-F84A-95E0-3F45AF32E19B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D604927A-92DA-2C47-9F61-7BA7D167DC4A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1B314E75-7B2A-8C4A-9841-A6BAAC8591D6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F8F02CA5-0BC8-DD48-888B-E5E35089D305.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BCC3B2E7-2FDB-5248-861B-31B8274CE873.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/810E4F76-D053-F34E-84DD-216E923CE53A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/689034DF-511A-B64E-8560-71658510D63B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0187D20B-9C4B-B446-96F1-A8E2A7CD39EE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5859D109-7940-0946-897C-C2CF6E2C0849.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/626A9C5D-1950-8042-AA9B-C9CE17E83A17.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BCCDB5F2-6134-1A4D-9E3F-98E1C110EF16.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D5D7C541-3020-F146-B888-6ECCD3F09639.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E2065954-67B9-E049-B61C-71914F461520.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E17D5ECD-6A45-D748-9364-003356FEFEE7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/49158DD4-1981-C846-9270-88FFB677FEEB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C3218B27-4921-7741-9357-0D0702E328FB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/38BFDEB4-6128-944D-948B-94B5FE0FB3D1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F25F02CE-3A45-AE47-9012-CFEBBC9B235F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/BD291243-6669-E64B-BB6C-8A63AB448899.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/26D026F8-39AB-8E4B-84A8-A166B164D956.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C8A941B7-9591-6E45-B076-90A5FE35450D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/CF6B762F-1F18-6346-A89D-670D642C4638.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1CFC7C53-610A-C34B-A5F7-CBCE67942523.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/68A8D9E8-8782-434E-848F-F3DB5C161301.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6881E5D4-AFDF-2D48-BA98-BEF64B2CFC1F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9379818B-13DA-8446-AC7D-BC3AAEAAF043.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4CE40267-CCB4-3740-87D6-883ECAA921EC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E034935D-0859-2A4F-ACF0-B8F1705240A0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/7478DDA4-9A9D-554F-B819-BE819EC07983.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/2D633B6A-DE4E-9045-AB1F-3661247B6AB8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5C862B18-6682-C946-8E54-3BCB165AB5AD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B712EAE5-E542-E44D-AAEA-5621E7F6BC37.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3D26549D-0E50-D541-8F33-6C3E42D5975E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/99FAEE9F-3400-FF42-9C19-29B2B8A02C86.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/51153708-45D4-7C4F-94B7-B4A229221A41.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5B66269B-DB1B-F94E-988E-70A90509A0B5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/35910BB0-C132-ED48-9C6B-665A83FB7F7E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8810934A-86A3-524E-8FF5-FB42FF5BDD0B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3D078B33-BD6E-744C-B153-68B37872521E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D4146AAD-9453-584E-A85F-79AC0B1D676F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/013D3114-DBD1-FC4B-BBD5-7E3D036A9790.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9F2CA230-F6C4-A942-9034-19DCD80EDF7D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1C43D47D-814A-0C49-A5AC-7B2A5E05C4FB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/622B730C-005B-164A-A7C6-9CDF871298BE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1F4DCE46-AFED-1749-A281-93650612D9E7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/95C752A4-9C39-AF46-8DC3-D07004387ED6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/EC47FABE-8245-7C40-A828-58A63D68E5B4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E85E0F42-4D02-8C45-9C00-5E30FA1C312F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F45696F3-3C98-C947-B996-C94E45D201C0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/09673D14-995E-6947-B3F6-6856AB0C12EC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/17E126E6-1C3C-1B40-ADF9-87C0F45D6FBC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4A107040-E585-9843-A970-DB49412EB891.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/43A338AA-384E-6D48-92D2-51D63FAD7E18.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1147F41F-EB0E-C641-9B3B-F4216708499D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6DAB3AD7-9D6E-014B-918D-B5C33D106011.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/72AC4100-ADCF-5042-9093-CD4CF62CBA26.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/90E0ED99-1792-6A41-9C3E-C6AB1F076CD1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A1984D22-C81B-E743-92E7-B7BCE3416481.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8AA1F7C2-E276-E849-A20D-00F6DDC97FFD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5D048550-9FE8-734D-B9C3-94792F9857BE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/AE58109E-B54A-EC43-A268-B8CA518B1B2A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6B1D429A-EE9B-7945-BE39-A4145B6D38E7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C21CA046-92D4-3042-B5F8-9190C35924EA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E6EAADE3-0497-0745-83C9-572E2A507188.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C34C4F9A-E8B2-B547-9D3A-9FEC7AC577DA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/66A4D0D9-6D06-2541-9712-17CD19F7573E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B3357BBF-0AA5-824D-AB53-CB6C88258C98.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/14589FF5-C583-1741-8D58-34DBEA27DAF2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D891CEEF-A99E-B345-80BA-D0FB99D8444E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0982C30B-3519-A642-B3A6-97D56E906962.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4BDA2E8C-6052-DF4C-BD2C-5C627A6B1B7E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A8E10598-9798-7F41-BB8E-3939ACE4378E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/34432450-70F3-D444-AEDF-C70D9A5523FC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0C6BB261-80FA-6E41-9FAA-993F3606A740.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/451788D6-C977-2A4A-A745-A5D1DF175023.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C8A3B98F-142D-3E41-840E-917D19A98C61.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A30F883F-71E9-904F-A034-DDD693DA5CA6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9CCC1EDF-96B5-CC48-9DC3-16F928841B67.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/47E0FD83-1555-D146-87E4-A6A570E4E64F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F882290D-3624-7E49-B26A-E6A9C23101BE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4A4F9092-2122-C940-AFE2-3D1BADE9BDCD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5CF8EA1A-A2BE-4648-A35A-7005F2B0D273.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/75A5A1E8-4CB7-3949-8627-3D33D20F5FFD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CC04D15E-5296-F641-ABB6-17D2771727AC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C6D7FF21-7FF7-154B-A921-634138B3CE30.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3CEBA7C4-BFEB-884C-9CED-B689A192CBDF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6176AC4B-130E-194A-BF72-23B0FDF0B67E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/50D53F29-A868-3241-B2DB-990E7BFF7AFA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D37FC913-48E2-D14C-A67B-3A841BDC83A4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DE5B1818-DAD9-9640-AB4A-647A5456384C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1169FA4D-9BE3-F44B-A415-AD0A98691137.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2649E864-9403-5F42-8574-1650D12D6E6A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9CCCE9B7-9C55-7944-B51D-E31638970F95.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3FE81576-3A40-9F44-B941-EF7DE3A21A2A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/33372A0B-A472-C142-A2D5-0295FDBC52CE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B2D25916-8CE3-4D4C-B620-C2DDFBB4CD9B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/907317E3-7E3C-AB48-8F3F-900B4D4EB3B1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/36460972-7174-824A-94C8-8993D0769E8B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/7C610B21-2A13-E047-B004-4A1DA60C56B5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/43F8400B-9C5D-B64C-81A2-1ED82BD4E921.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/53FC3FDC-D015-6046-AB91-C0CCD32E4B25.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/DB7E6FD1-7BE6-994E-8295-E833A529C872.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/091CE3DA-B2D6-CC47-B8C4-A5ECD8CAB3A1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6EDB1A0D-D5FA-1B41-9BE1-D43F0C3EB347.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6E3BCC8F-4688-DE46-8CFE-E35F9D4F96FF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8A00D54B-FAA0-1C4F-B1FA-8C71404A700A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8F287512-732F-F043-B26E-AC6E39B36B4D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6255EF2C-06F9-4547-A173-44C47E553840.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1933EFA5-0010-5945-8A23-8FD670DD7EB5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/AA29A70E-8F8F-FA4B-BE9F-076438339616.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A0CA08AD-119A-0B44-BC22-DF2CC1CF90B8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/690E25BF-E4A7-C84B-8BE0-2CC19CFE0C2B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1921372B-64FA-CC4E-833E-4E621BC72992.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/14216260-5C65-0F4D-A1E6-B948F172EE6F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/36E66EDA-2A47-5747-9EDD-CA32DF7262D4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C4531D02-6639-7140-B7BF-EF4B6A533E95.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4AE1B52D-84EA-764F-8DF5-CD386D0218FD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/CB362136-8567-AF46-AC35-CA54C6A82A13.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5FA35A20-1FF7-9D42-8151-D90EC85734CD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8D308388-8EFE-C748-8E66-94B0964665DA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/37252A85-1A95-9F4C-9B92-9914004B88C8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/24197C8A-A66C-1247-AF32-89B693A24984.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/EFB7E650-D855-C942-A33E-B776E7F5975B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/812D56F0-D8FD-0B49-ACC1-1B3854507B75.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B144D859-1173-454B-BCE6-7B46CC08A43C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5FD60D83-D038-BB4F-859C-D8914AB09B5D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4D5522AC-8CC2-EE49-86EA-001E89A1DB5B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0F7B9F25-EBAB-6F4F-B432-877F52140336.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C13BB617-EB46-9E4D-ACB3-D4E48C8AB022.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/FEAEAE4B-B0A0-8B47-A807-40DD2895616F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3774EA20-66F4-1D49-824D-51158010E93E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/04571F33-E180-F04A-8BA6-56032E4DF30C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/22A6CA2B-364B-5649-9347-216BC8767540.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D17EB0AD-546E-B54B-8081-CB60BFAD6290.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/746D679A-CC48-534A-A545-73FD8A53437C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4BE7B411-87B8-E04C-B85A-3D5793DC9E5B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/85D5F95B-B0B1-7F43-90AB-4566C9D12412.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/449634D0-0234-D14C-946F-AB63DD492EC1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/CE46B199-0B87-8746-8060-91A4EB2A6D22.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/BCCC9100-C282-7D41-9DC6-CFE739E8D7C2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B7896E7C-FF74-204F-8023-D6FC90DF4004.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/26424E25-53C2-FE46-8910-A250A7C6EC03.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/444BE35D-B858-1546-AF06-BD218A82FCE1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B32A8503-0C6C-C747-AE65-E0E46103C783.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5715E2D8-7F54-B643-82DD-BD6DD0281B4A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B39B2EFE-85D7-9B4F-8CDE-09BD5D075F5E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6A44BA6B-3447-4D40-B141-8F336A0699C6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E790B5FE-9573-8C47-85FB-A6B4ACF2F52B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C724932D-5C96-EC40-8936-5FA9B3040425.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/88FA2BDF-DF6E-AF43-9B0F-07E8EF463EF1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D3132BED-6728-BD48-ABDE-B8228DEAA15E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/11F3D8DE-C227-A843-BF60-8D59DDF10ACE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/09E31383-3A3D-2A4E-8CF7-F747C2A2424C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B29EE45C-71B0-D744-B502-8C135595C0F9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/367127A0-51BF-F34C-B93D-AB5EA5985A5C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/31FC6D64-4C61-4D4E-9F28-D2882C30748A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AC4833CA-2EC9-C442-AAF7-50FA0FA24582.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4D900CA6-C310-4440-9B4B-30AB82260E5A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/270C8B87-C0DD-5A4C-A227-D44D90E06F3D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0AAFD32E-0747-5A46-80F1-30C85ED77268.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5853C73A-4207-6B48-ABD1-933E9A6BB7FB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A814FADE-5A65-E549-820E-BC4636D0DF7E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/53DB5827-BECA-C446-BB2C-8AE15A0D1AC8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9C9C2AD9-4BB7-874D-AD66-B3E17653ED1D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D5DE7E0D-49F0-F240-8435-D796578E94F3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A0A3DCF2-D50A-0741-AD58-0FCC96317FAF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A0519088-7BC3-FE41-8711-205850F11B1F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DFEB6A48-72B0-144B-BE07-9D13506F782D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2EBADA63-4D67-3C41-B23D-18C0250AB5F6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E82A9960-5BF2-5540-A1DA-2455D1B6B52C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F3B79F95-E9ED-5548-9127-13654D6F8AC0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/938F9E79-7D26-6546-9064-A664E42531B4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9B38C261-A3FA-3749-953F-6DC556208E84.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D603B4D0-8682-9949-941B-B29FFBC89728.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2F267A9A-A084-F945-99A7-EB81DF5DAE3E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BABF93DB-2539-1044-890B-0524A5A47A01.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/79CBF0D5-D33A-AB43-8B38-B36514D7543F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/43C11108-C681-3049-8796-84510A281266.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4D9290A4-B056-D741-A5E6-7929DEDB202E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8FA8072E-3C7D-AB4E-93E4-7890315CD902.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/050C0E05-7B05-2241-AC5A-D4BC5A324D23.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D717AA2A-51FF-9244-A9E4-9CEED4BD47EE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9857DE8F-DA46-284A-B175-58D9951B0B21.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C69A3520-5099-A343-BD8C-C444204D8338.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3C374EA1-5A7D-5546-A0B3-9CCB82793D1B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0BCE74D5-48CF-D546-8FE8-48C739C35B5A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/02F3E0E0-8560-0649-B10F-1FD4706BFC85.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B64616E9-34E4-FE4D-973D-57A133513C5B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/2ADC9148-F23F-054D-9507-4EF8A1A202E0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E055C561-1BFD-F345-A860-E678F6876CBD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9F3F59C7-7992-3448-841A-44488664145A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4AD99120-E44F-1D47-BCA4-5EA63D78AB88.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A9527C6A-D43A-864C-83D8-9DAEA6D800CC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/CE881F1E-60E4-BF47-83E4-6A036EE8B451.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/05F43B5F-8756-A842-B4F9-5DDC47E7EF84.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C4DD9C4B-4B8C-B947-895E-F431AE8E2BAB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/96A96E0A-E954-6F4D-8CE4-CE50400B5275.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3A930ECF-9CB9-F448-A2EC-45A0E85B9A30.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1FE148A1-560C-A142-8DD6-9B7F2D8E002D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/90B04344-FD67-BB4B-9175-6BB2AB35AE62.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A46C1CA6-E218-6F4B-9452-CA0AB43FF233.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/538A28D4-E41F-A747-BC97-3206F3CBEB22.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/3EEA5DAA-1599-C644-9E14-8C41FEA9247E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/526A3794-3EE6-8245-83E0-B2B88702C9EF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/ECC9A098-BDA6-B24C-9AF7-70AB4DEE30EB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/38328879-C636-C840-A931-A50B3F401A6D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/DD9583D2-EA39-4F48-AFC2-09D95A1DE07B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B38D4592-2839-3645-AF02-FE014C17A11B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/86F04269-3DB5-9842-8A34-6CBFA32D8383.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/357E63FA-BA16-4D4C-8A76-AFC5BEE69D18.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/97CC1975-FA4C-A842-BDB5-55058B78789F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6DFF922A-6069-B747-9136-E37009E2FEFE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A1FAFE45-C43F-D844-9B45-32AB74CFBED3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/937C5D66-0F80-3D49-8111-4B51EA5FB4E3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/681FFFEA-8B9E-384F-9E26-A254F4814071.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/81C2732A-97C7-D94B-96E5-8B1C29DF8720.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4765C051-2E48-7D4E-A82D-A49BC3F867F9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/06086276-5AFD-6A4A-8AB1-14B67C92750C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D077FCBC-DCCB-464A-8D28-5759A38A6A28.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A496C07A-4F72-184D-971A-292B3052CC94.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/7D863F16-5921-7F47-83E8-647E79E84B9E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/DC818A3A-27DC-D342-9D87-EFD5752397A5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1BD54385-AB74-2F49-834A-8B643BD3C5CA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/40E5DF15-3EE8-EE43-BC62-C72BB7296737.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F5D05EBE-ACD7-9B42-BD88-19F54A604622.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D57F84B9-2FF7-D94F-A1AB-E0106D2D34E9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/326F60F9-9343-D046-8AF2-C66E6FC66D07.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6C70EBD0-1967-5343-86BD-A8FD566B1156.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/8428EC8D-240E-964B-82E5-8F90E692DB9A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/FC4C6742-0C8C-4D4C-B32E-BA95B2A45F08.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/C90F9DA7-20B7-D24E-9A53-20CF8D4D3E9F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/198D739D-860C-6948-9719-74A57D5A53D0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/38AE5F2F-C354-BB47-A452-6C1A73A6869B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D9BEF431-71F8-004A-8D74-8F161A4AC4EA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F03E5B36-40FB-604F-829B-D5987EF9FFA3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AC82891C-B2A3-5446-86F5-D9ACCC4A02C9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/87E84C06-334C-6D49-B478-2CA77E335A79.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/89645C0B-4092-0943-A306-179F2FC3C0F5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/5FE6ED94-EAB7-9A4A-AA97-9E6B58783792.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/EE913264-992E-EB47-A61B-4AF8A3D60BE5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B4F5A565-34F1-C24C-BFC6-FC7C37077E26.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/FC72F2D1-BE99-B44C-AF67-09606BAB1011.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0068723F-CD76-3E46-AEC3-1A83FEC92474.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/4E194F9B-4322-3440-AC80-820765373942.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E7662D89-BEBF-B640-8D90-1EBD8FE6CDC3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6F84777E-3C2B-514A-BD07-2BA2370D505A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/77B8A08D-4E37-DB44-BB05-7113CFC8DF2F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E9A0DCA1-3ED2-4C40-8870-D619E521F364.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/D9AD8820-C4CC-254E-90DE-097901E0966E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F77D359B-3231-CB45-997C-C858ACD8FE61.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0249E66D-3B18-5C47-AC68-F87399C75C5D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/0222AB99-23DF-854A-A4E3-AB739D4ABAF8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F96445C3-73FC-3D43-A83E-7C0738C866BB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/9F71D97E-FD22-F04C-8763-C725AC76B6A7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/26C62523-5285-9F41-A58B-040A7E6E873B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/E68EF427-80C2-6D40-9EEA-EF14B443E223.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/B51191D3-CEC7-D54A-888E-0B83AF7F727E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/564022BA-693D-0746-B0A4-C397141F5630.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/FE360A75-48AD-3A40-83E3-B24D2CFEF85D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/CD43D675-C4E6-0A45-AB46-A34E6CDA0626.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/CA4313BC-CB7B-FB4C-A473-A8CB3E7DD276.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/1925EF8D-D89E-0940-8F15-714AA722F02E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/044A41DF-F3C0-C84B-A1F5-8A71DA592CE7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/F42323CF-B486-DF4B-8E65-555E34C01517.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/79A678C9-9ADE-4C42-93E2-194439A74968.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/BD4853E6-5BB5-1A46-ABE0-F114B83AB685.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/C2F65C10-C7D3-FF42-99A1-0448E3113BE6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/2073F5E3-1862-504D-86C4-6D4DF1B92A2E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6154BC4F-8D1F-9A49-9CA5-B4FE4798801F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/03EECA56-1714-7A4C-8998-3ACC8E7F98A7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/1E3A23D0-5F01-DC4C-B1B3-82477A0E09AC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/B8522242-4414-1B40-AB5F-A835DF6EDE98.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/12EE6A76-7FFF-9740-A764-12DAC3DD9076.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/EBD7C33B-017B-E54B-9C32-DB1314188B6D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/6733E9FA-2212-9349-86E4-1CB421F64136.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/FBE9CB94-F6E2-DE44-A3B6-4A215A7E5F01.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/BB507515-4B12-8147-A7B7-3CAF218ACB81.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/041E06CA-069C-2242-A60F-0DB0C641FEDD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/20AE5FA2-7DF2-BF43-BD50-1C8638FE677F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/917A29D6-D4ED-4444-A4AF-5E60AE0C90AD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/735A8632-B8C5-7543-81C1-20DA46CDF69F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/59B3CD44-4B82-9C4A-8280-CE515970E18D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/6AEBBF17-BC1E-644E-BD6A-3BBC09B761DC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/A2EBC9B6-05E3-6D4B-B89A-FC9593F188CC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/4ABA727C-D40A-4744-9A0B-2EDA83BDE84D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/33095637-3709-B448-BB78-D0402ECEEF6C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/DE6B66CA-CFD9-CD41-AC86-55907BAB27FA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/7965F491-E2FB-A842-957B-FCC3724EB2B9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/E4C9A04B-3A3A-7245-9007-BC331AA72927.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/2AF415ED-CAC6-5540-A4DA-234A59EE0CB7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/71366155-05A3-2B4A-B8B3-97B66FC76CFC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/D229D307-2A41-B649-8870-04BFB61842B7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/DDE480DE-4C49-4247-86F6-E12BAE1B895B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/BD76E91E-70C3-5749-9BCD-312D614BF026.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/52C7CB1D-1948-7041-B2B7-EF4BD6B51B6B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230001/D897B6BD-3E76-7340-AA8D-2EFC757E4DAD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/230000/20AE3E48-BB49-9444-BDC1-B9126AC49BFE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/28A6686F-03DD-3349-8261-CAAFC222257B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/75312BEA-32BD-6E41-A1D2-2FEA56F61F75.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9D67720C-B781-BF4D-BB4E-A901D08A9307.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1D65CFC9-D069-B340-B8E2-2CDCAC4BC8B5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/56CB729C-1D5E-A845-A3B7-4B99ED327625.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/964B7100-9AE1-2749-9FC4-6C679A8C0367.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/459576CC-0428-3646-A85E-0B41048269F9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2BE8D4BE-940E-8F45-AFC5-DB98B86B31CD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FB5255EC-7E9F-7D45-8F61-BB2FEE2690BB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5F19755E-4749-2140-8AD5-06CB61DB0AA7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4F310A38-4FA2-2B4A-9682-D236BDB9E440.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/33B193BE-0B08-4247-B041-E1D4ECC9EC5D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8169C221-E11E-6544-B477-A6EFAB989306.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AC3ADA39-660E-C74E-8316-65F900AA5CA2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CB101052-A346-4C4F-97BB-D69A3FDD5F8D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/749DCBFA-4916-7E4E-AD22-F658973A6988.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/7F4E273B-6E43-D44D-B72A-EF41EBA2DBC2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DD06C6E6-2268-BC49-8B0A-554B1DDEB57D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/26745664-2AF4-7141-9C9E-292B0A74A704.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/34E9C712-AE91-5D46-AFD6-80EAB623922E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DA71C779-1B7C-3946-8DC5-06CA0843AE06.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B0C9C247-A11E-7444-840A-7CDDD26B64B0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/7E106CB1-CD77-9B41-B512-08DB7B940BCA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D831BAC0-C502-2247-A4A8-39AFF1703744.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/012C455C-B20E-5242-88CB-9DDBFDB14841.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2D2E3433-8222-BC4C-8E3C-2EEB7C8A6195.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/67517132-9FCB-9E49-B4F0-856595A70F02.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C5E4628E-8201-914D-ADE5-94A380FFE932.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D0B77B6E-A682-384B-9B96-9146F5C55EC4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/46AFE019-1379-4C42-BA5F-1FC9719D1EDD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/68B87E4C-5607-0C4C-A8AB-39BAEE7A4FAD.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/53AB0542-4F7B-0A49-9E03-204A59603F91.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B4DB2B09-DC7B-4944-986A-2D803F4D3BEC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A3E13B34-5E99-C64B-90DA-4E3549F1CCF4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C45412E8-2B9B-5745-8126-04571EC14FD4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C4A05712-56A1-C348-9DF1-66095D2CADE0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/92E1C21E-5289-7B4E-B471-DBD3DC296E1E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0CD92960-49D3-C34F-804B-7BA86D2F5B78.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1B67DAE5-C6B2-704E-8735-20722FEB5530.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1A214FC3-7AFF-3E4A-8381-15EA975D9BD3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C3664824-E330-574A-A0AE-3B78EB8FFD3B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/62531B99-FDAD-1E46-A3BD-3462C3F0E6F8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/ADD976CC-AE7B-4546-984B-2EC312E2E39A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/892BCC26-AB8E-FC4D-B5F8-E97760F4B852.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/85104BCF-3802-7442-8897-A0E4EBCAFB4B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4C60A1B1-E3C7-D84E-8FA9-C1C8609F0794.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8557E827-E30D-3D42-9F15-2A5A1F4333F8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3B569C97-F640-A242-BD5B-5BBB115EFC66.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C84B1D7D-927A-2F44-9D86-99F1AAD8C630.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/ACF6B9D2-F8D3-8D46-9558-66F0A66DF77E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6A209213-774A-4C42-A890-B857E6AF08BA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/312347F7-F853-4C47-BE03-656A4BE2887E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/50465C91-A7C0-0C4E-98E0-37157F5E0F64.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9EA3D2F1-8F49-6C4E-AB5B-863C2EAFD98F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0A3A0FC7-704E-9743-BFCE-860A9FF5DF50.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FB83FFDA-96A7-5F4C-B16C-6A1D93880091.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/7368EC7D-374F-0742-9012-4CFC641FC09C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/38067A10-2814-5745-95C6-8359AFBD93A5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6315ACCB-3A7E-3245-9926-7C00B59A9A42.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6D7E7835-EDFB-364D-BD2F-17029F2F34B4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FAD38FAC-295C-6B48-A2AF-3AC035BFC629.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/499198AB-2957-9D43-BBF9-4AF34FE7BCC5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2D5C0DA4-887F-9243-9D34-A4A5C2BDF1E4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2F265613-71C5-BB46-8E96-C15FA725ED9B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AD12F4EB-E5A9-1042-B56D-988CB1E9FD7D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5553BDF0-D595-904D-B927-E4654B68097C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C72DB50F-1953-AD4E-85A2-7B50621BFCA8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/75517AB4-BE1C-6247-B6B5-CD8FFE9595C3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8DAC82FF-4643-AF4F-ABFA-F7CCBF11C414.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C2017799-7F32-F04C-A0E8-AB1020B355E8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B470FE6A-F9ED-AE43-BB73-54152E19B89E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BED2A676-E014-2242-AD13-CC61DE8A3575.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E0490388-4A2F-F840-ACA9-0FB08FF58B6E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/397CBB8E-6399-384F-9DA4-CBDE7D572F80.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0151C652-1B60-F843-A691-AD3A4AB5C8EA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D9AAE0BE-0516-8A48-A094-F1E9996D721F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FEEC66E6-48D1-6547-9E42-9EBEC64EEC02.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4E53941D-1EED-C947-BAFA-BA35B66A5103.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D72F02A5-0B58-024E-A3D7-4279A94CE3BC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C90E118A-12DA-5947-A9D1-C268362F7E54.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F9B01FA2-2B66-7A46-B308-289BD181C502.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/595E31B1-2C4C-1444-B3C8-F19D335D691E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6940C5E6-E20D-7D4B-A422-74821E22D216.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3C41C989-06E8-514C-BB6F-C362077419B8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/74D6CE38-7EB0-B148-91D0-E5B233D92D9B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1614DD63-D99C-F441-A9B0-9B335C0EBAD6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3D548BA0-E7BA-E04B-8E90-6CFB96E82EA0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/73EBA431-8254-C345-8BB1-3EE772F62917.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E38D0E97-C9AF-7D44-8A4F-55297555B038.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5DF05081-F2C3-B04E-97AF-858A8280189D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/7FE25507-C19A-E844-8902-702697A90C00.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F16A9895-C5A4-DD41-B79D-47D39A5E9D11.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/08577B80-2033-0240-8B8E-40FD47E24396.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/646B37C3-D3A5-A040-A77C-A664340F1F67.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CF959303-47F3-6044-A54C-3F4BD31F194F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/57DDAD79-AE48-EB4C-AD93-0EE998F6EE04.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/69C90830-E105-354F-8221-A4555E90D42E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/23625C14-52AF-7D4D-928F-7E1D634232A3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9EB54C74-5B87-6F4C-A766-EF43BAD0E3A6.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AEBBD4D9-9D9F-7145-8163-4FD8F4B82258.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A1C89248-016B-9043-9530-EDD13A6A6C10.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CF8AD0A8-F30B-8546-8945-76ABACD0D9F1.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2AB0D681-03F2-F94C-9726-C5D4A2665A3E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/375A4EE5-DB4F-B04C-8E0A-630CF605D3C9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4AB5D46F-8619-B240-BEDE-AA629A4E5682.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E052AB43-F1D2-004C-9572-57D69A218B6F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C43C42FA-8D86-4048-8B5A-0062F1FE6356.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6CA19956-F0D9-7D46-8EF0-7266BD3B7DDC.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/850065B5-7034-824A-B6B0-EC875466626A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4F633F4D-A758-5445-99AE-51834B0C6950.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/CEDBB8ED-DBA5-DF4B-88A4-882A415CFAEA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2FDB17D2-7582-BA42-9712-A6EF00666C7A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4DE99DA7-784B-4F43-BA67-B2C73744E89E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/B22E1A3D-7698-EC4A-9FD0-AA5DF3B2AEEF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D37DC2AB-9F3F-494E-AE4F-ED7D2933E2F3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9D5AA0D1-EDD2-8445-B788-2177E1D8E961.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E44C9A04-12F6-E840-B49A-6311EF2AADA7.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5031ADC3-7E11-414A-8730-29B6212AE394.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/750041AF-38AE-1545-8453-C2F8C024F926.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/71A60FBB-05F8-4045-B542-47A5B674F841.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/0A4D9D5D-A76C-AE4A-BF92-BB3DD0B8C38A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/7D70895C-2EBC-CF4F-BD49-B72FAAA2B204.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E2E1E337-A87E-3943-92CA-7A0882754FCA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6033B390-CC7E-8A4B-8BCC-1DA1F8E617AB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F158B6F1-ECDB-4A47-966B-FE3C6B986125.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/19826063-5174-C240-A1C8-824242177FC8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/969694D2-368B-D245-989B-9C067DB97FE2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/4AB6C29D-928B-6B48-BD00-0ED50A659544.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/64BDDADA-6CB2-4049-B4BE-B443102FE8CB.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9CD7F2F0-CB5F-A944-A436-50603B031DDF.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E0B34468-2CFE-1F44-BF82-F8548C0FD3ED.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A37BB1ED-1CA3-D547-A3CA-912224EFC4DE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E3B63211-AAEE-9C43-B4C0-89E82C7F2E0D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/34CD830A-3556-2B42-AC50-0F0B824585BA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/12649DAD-9F01-1843-8DF8-69D4BA0DC7A8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/5B22BE1F-FF91-0345-85E9-0C6272A32AE4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/481DA80F-1981-B149-94CA-6AB36B7D756B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D0C0EF9A-BEA2-A948-B042-3AF181D7D781.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1CD16636-E3E2-B94D-8201-8D6BCB4DE131.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/C202A4C1-69BC-1442-AA74-86A0144803AA.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/71762F7E-C85F-E441-ACC3-388E3111AB8C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/2DFB0BE9-703D-894A-B174-8A54B46EF464.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/F6F61B9C-0CE9-AB47-9F40-FCDEA28417B5.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6DE71244-3A94-7B42-8E62-A067ECAEFC8F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/AB1CEE81-FAA6-CD4B-A9E1-4FE16F3B51F0.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/1BCBC723-86A7-A543-8716-8360E79EDB2E.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E931C781-C4D7-9440-B4CD-F6260526CE4D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D32E04A1-108A-3A40-8B2F-5CFEB0655D86.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/52C6B1A1-0FB7-4143-8974-A61AA847E4B9.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/34968649-58DF-7347-A815-C84A834C7486.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/A5E92A3E-B74A-3842-939B-E3C653D2295A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/77866CF4-FFC0-D741-96A9-0C3F219CCFB8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3150DABD-8B2E-DC4E-9EE8-8FF9C925C08F.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9D229D5F-7094-F54C-BDE7-A433DE8E63D2.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/26349E50-9299-B54C-BE7F-1C06BF6ADE3A.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/66CAF5AB-3F20-D74F-9EFF-E2C24D0E5C05.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/BEFEC187-3580-0640-B00A-EBA34BE34169.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9296105A-0739-5C45-ACDE-FDFFB890C3C8.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/6EE9CD78-D16B-1B43-93B0-4781B53549E4.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/3521C0E5-697D-9E49-9766-D6A3B9EA5D39.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/86E772B9-C22A-094A-87D0-55F282EE30BE.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/DE518419-804C-8E48-A1C3-A0B842207841.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/9B37F219-0D75-7946-8911-3AFCB87B610B.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/042CBA7F-E2D2-074F-83F2-7644122AA010.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8F814ACD-4514-9440-80AD-49F6B131AC14.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/64249BF3-2038-F245-91E6-FFE870E6312C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/E47B2962-E79B-1A43-9AAE-4F1E341683D3.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D123E33E-37DC-FD48-A1C3-B44A6A54A21C.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/D32734D9-E45B-6244-90B2-C2DF9258E021.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/FAB6D7C3-E85B-8942-8501-8FBCC8BE539D.root'
'/store/mc/RunIISummer20UL18RECO/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_upgrade2018_realistic_v11_L1v1-v1/00000/8166F723-C720-3646-943F-4CA58B03C4BD.root'

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
    fileName = cms.untracked.string('file:step2.root'),
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

