import FWCore.ParameterSet.Config as cms

################### CONSTANTS ################

#geometry from https://cms-docdb.cern.ch/cgi-bin/PublicDocDB/RetrieveFile?docid=13251&filename=20210803%20HGCAL%20PARAMETER%20DRAWING.pdf&version=11
HGCAL_MERGING_PARAMS = cms.PSet(
    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),
    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1)
)

#ibid
L1THGCAL_MERGING_PARAMS = cms.PSet(
    caloR = cms.double(136.5),
    caloZ = cms.double(318.5),
    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1)
)

#geometry from https://cms-docdb.cern.ch/cgi-bin/DocDB/RetrieveFile?docid=3661&filename=quarterView-HBHE.pdf&version=1
HCAL_MERGING_PARAMS = cms.PSet(
    caloR = cms.double(180.6),
    caloZ = cms.double(388.8),
    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1)
)

#geometry is a guess. would love to find a real diagram
ECAL_MERGING_PARAMS = cms.PSet(
    caloR = cms.double(135.0),
    caloZ = cms.double(310.0),
    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1)
)

#ibid
ECALHCAL_MERGING_PARAMS = cms.PSet(
    caloR = cms.double(135.0),
    caloZ = cms.double(310.0),
    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1)
)

def setupSimTruth(process, subdet, verbose=0):
    #lookup parameters
    if subdet == 'ECAL':
        params = ECAL_MERGING_PARAMS
    elif subdet == 'HCAL':
        params = HCAL_MERGING_PARAMS
    elif subdet == 'HGCAL':
        params = HGCAL_MERGING_PARAMS
    elif subdet == 'L1THGCAL':
        params = L1THGCAL_MERGING_PARAMS
    else:
        raise ValueError("subdet must be one of 'ECAL', 'HCAL', 'HGCAL', or 'L1THGCAL'")

    seq = cms.Sequence()

    #setup list of simhit collections
    if subdet == 'ECAL':
        simhits = cms.VInputTag(
            'g4SimHits:EcalHitsEB',
        )
    elif subdet == 'HCAL':
        simhits = cms.VInputTag(
            'g4SimHits:HcalHits',
        )
    elif subdet == 'HGCAL':
        simhits = cms.VInputTag(
            'g4SimHits:HGCHitsEE',
            'g4SimHits:HGCHitsHEfront',
            'g4SimHits:HGCHitsHEback'
        )
    elif subdet == 'L1THGCAL':
        process.TCSimHits = cms.EDProducer("TriggerCellSimHitsProducer",
            simHits = cms.VInputTag(
                'g4SimHits:HGCHitsEE',
                'g4SimHits:HGCHitsHEfront',
                'g4SimHits:HGCHitsHEback'
            )
        )
        seq += process.TCSimHits

        simhits = cms.VInputTag('TCSimHits')

    setattr(process, 'SimTreeTruthMerger%s'%subdet,
        cms.EDProducer("SimTreeTruthMerger",
            simtracks = cms.InputTag('g4SimHits'),
            simvertices = cms.InputTag('g4SimHits'),

            simhits = simhits,

            caloR = params.caloR,
            caloZ = params.caloZ,

            verbose = cms.int32(verbose)
        )
    )

    setattr(process, 'OverlapTruthMerger%s'%subdet,
        cms.EDProducer(
            'OverlapTruthMerger',
            simvertices = cms.InputTag('g4SimHits'),

            simtracks = cms.InputTag('SimTreeTruthMerger%s:mergedSimTracks'%subdet),
            simhits = cms.VInputTag('SimTreeTruthMerger%s:relabeledSimHits'%subdet),
            simclusters = cms.InputTag('SimTreeTruthMerger%s:mergedSimClusters'%subdet),

            caloR = params.caloR,
            caloZ = params.caloZ,

            overlapThreshold = params.overlapThreshold,
            distanceTol = params.distanceTol,

            verbose = cms.int32(verbose)
        )
    )

    seq += getattr(process, 'SimTreeTruthMerger%s'%subdet)
    seq += getattr(process, 'OverlapTruthMerger%s'%subdet)
    

    setattr(process, 'SimTruth%sSequence'%subdet,
        seq
    )

    return process

from RecoParticleFlow.PFTruthProducer.SimClusterTable_cfi import SimClusterTable

def setupSimTruthTables(process, subdet):
    setattr(process, 'MergedSimCluster%sTable'%subdet,
        SimClusterTable.clone(
            src = cms.InputTag('OverlapTruthMerger%s:mergedSimClusters'%subdet),
            name = 'MergedSimCluster%s'%subdet
        )
    )

    setattr(process, 'SimTruth%sTablesSequence'%subdet,
        cms.Sequence(
            getattr(process, 'MergedSimCluster%sTable'%subdet)
        )
    )

    #process.schedule.associate(
    #    getattr(process, 'SimTruth%sTablesTask'%subdet)
    #)

    return process


