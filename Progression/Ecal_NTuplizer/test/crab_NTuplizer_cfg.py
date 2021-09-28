from CRABClient.UserUtilities import config
import FWCore.ParameterSet.Config as cms

config = config()

config.General.requestName = ''
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'

config.JobType.outputFiles= [
    'nTupleMC.root'
]

config.JobType.psetName = 'Electron_RecHit_AOD_cfg.py'
config.JobType.inputFiles = ['DBFiles']
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxJobRuntimeMin = 2750

config.Data.inputDataset  = '/DYJetsToEE_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer19UL18RECO-NoPUECAL4BX25_106X_upgrade2018_realistic_v11_Ecal4-v2/AODSIM'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'FileBased' #'LumiBased'
config.Data.unitsPerJob   = 10 #30000
config.Data.outLFNDirBase = '/store/group/dpg_ecal/alca_ecalcalib/ecalelf/ntuples/13TeV/AODNTUPLE/106X_MCRun2_UL18_Ecal4/DYJets_M50-CP5-madgraph-Run2018UL-MC-Baseline-BX25-NoPU-NoNoise-NoThresh'
config.Data.publication   = False

# GRID
config.Site.storageSite   =  'T2_CH_CERN'
