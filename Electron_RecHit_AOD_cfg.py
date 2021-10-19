import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(2500)

options = VarParsing.VarParsing('standard')

options.register('inputFile',
        "~/",
        VarParsing.VarParsing.multiplicity.singleton,
        VarParsing.VarParsing.varType.string,
        "File containing a list of the EXACT location of the output file  (default = ~/)"
        )


options.parseArguments()
options.inputFile = 'root://eoscms//' + options.inputFile
print(options.inputFile)
process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
'root://cms-xrd-global.cern.ch///store/mc/RunIISummer19UL18RECO/DYJetsToEE_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/AODSIM/NoPUECAL4BX25_106X_upgrade2018_realistic_v11_Ecal4-v2/20000/D2F029DD-CACC-AA48-BC51-D1987DD92E26.root'
                )
                            )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2018_realistic_v11_Ecal5', '')


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



process.scEBEnergyCorrectorProducer = cms.EDProducer('SCEnergyCorrectorProducer',
   correctorCfg = cms.PSet(
     isHLT = cms.bool(False),
     isPhaseII = cms.bool(False),
     applySigmaIetaIphiBug = cms.bool(False),
     ecalRecHitsEE = cms.InputTag('reducedEcalRecHitsEE'),
     ecalRecHitsEB = cms.InputTag('reducedEcalRecHitsEB'),
     regressionKeyEB = cms.string('pfscecal_EBCorrection_offline_v2'),
     regressionKeyEE = cms.string('pfscecal_EECorrection_offline_v2'),
     uncertaintyKeyEB = cms.string('pfscecal_EBUncertainty_offline_v2'),
     uncertaintyKeyEE = cms.string('pfscecal_EEUncertainty_offline_v2'),
     regressionMinEB = cms.double(0.2),
     regressionMaxEB = cms.double(2),
     regressionMinEE = cms.double(0.2),
     regressionMaxEE = cms.double(2),
     uncertaintyMinEB = cms.double(0.0002),
     uncertaintyMaxEB = cms.double(0.5),
     uncertaintyMinEE = cms.double(0.0002),
     uncertaintyMaxEE = cms.double(0.5),
     vertexCollection = cms.InputTag('offlinePrimaryVertices'),
     eRecHitThreshold = cms.double(1),
     hgcalRecHits = cms.InputTag(''),
     hgcalCylinderR = cms.double(2.7999999523162842)
   ),
   writeFeatures = cms.bool(False),
   inputSCs = cms.InputTag('particleFlowSuperClusterECAL','particleFlowSuperClusterECALBarrel'),
   mightGet = cms.optional.untracked.vstring
 )


process.scEEEnergyCorrectorProducer = cms.EDProducer('SCEnergyCorrectorProducer',
   correctorCfg = cms.PSet(
     isHLT = cms.bool(False),
     isPhaseII = cms.bool(False),
     applySigmaIetaIphiBug = cms.bool(False),
     ecalRecHitsEE = cms.InputTag('reducedEcalRecHitsEE'),
     ecalRecHitsEB = cms.InputTag('reducedEcalRecHitsEB'),
     regressionKeyEB = cms.string('pfscecal_EBCorrection_offline_v2'),
     regressionKeyEE = cms.string('pfscecal_EECorrection_offline_v2'),
     uncertaintyKeyEB = cms.string('pfscecal_EBUncertainty_offline_v2'),
     uncertaintyKeyEE = cms.string('pfscecal_EEUncertainty_offline_v2'),
     regressionMinEB = cms.double(0.2),
     regressionMaxEB = cms.double(2),
     regressionMinEE = cms.double(0.2),
     regressionMaxEE = cms.double(2),
     uncertaintyMinEB = cms.double(0.0002),
     uncertaintyMaxEB = cms.double(0.5),
     uncertaintyMinEE = cms.double(0.0002),
     uncertaintyMaxEE = cms.double(0.5),
     vertexCollection = cms.InputTag('offlinePrimaryVertices'),
     eRecHitThreshold = cms.double(1),
     hgcalRecHits = cms.InputTag(''),
     hgcalCylinderR = cms.double(2.7999999523162842)
   ),
   writeFeatures = cms.bool(False),
   inputSCs = cms.InputTag('particleFlowSuperClusterECAL','particleFlowSuperClusterECALEndcapWithPreshower'),
   mightGet = cms.optional.untracked.vstring
 )

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
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

process.p = cms.Path(process.scEBEnergyCorrectorProducer*process.scEEEnergyCorrectorProducer*process.egmGsfElectronIDSequence*process.nTuplelize)
