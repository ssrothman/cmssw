import FWCore.ParameterSet.Config as cms

from common_cff import merging_params

def setupSimTruth(process, subdet, verbose=0):
    #lookup parameters
    if subdet in merging_params:
        params = merging_params[subdet]
    else:
        valid_options = "', '".join(merging_params.keys())
        raise ValueError(f"subdet must be one of '{valid_options}'")

    seq = cms.Sequence()

    #setup list of simhit collections
    if subdet == 'L1THGCAL':
        process.TCSimHits = cms.EDProducer("TriggerCellSimHitsProducer",
            simHits = merging_params['HGCAL'].simhits
        )
        seq += process.TCSimHits


    setattr(process, 'SimTreeTruthMerger%s'%subdet,
        cms.EDProducer("SimTreeTruthMerger",
            simtracks = cms.InputTag('g4SimHits'),
            simvertices = cms.InputTag('g4SimHits'),

            simhits = params.simhits,

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


