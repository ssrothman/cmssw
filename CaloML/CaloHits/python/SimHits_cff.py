import FWCore.ParameterSet.Config as cms # pyright: ignore[reportMissingImports]

from CaloML.SimTruth.common_cff import merging_params # pyright: ignore[reportMissingImports]

def setupL1THGCalSimHits(process):
    process.TCSimHits = cms.EDProducer("TriggerCellSimHitsProducer",
        simHits = merging_params['HGCAL'].simhits
    )
    process.TCSimHitTask = cms.Task(
        process.TCSimHits
    )
    process.schedule.associate(process.TCSimHitTask)
    return process

def setupHcalSimHits(process):
    process.RelabelledHcalSimHits = cms.EDProducer(
        "RelabelledHcalSimHitsProducer",
        src = cms.InputTag('g4SimHits','HcalHits')
    )
    process.HcalSimHitTask = cms.Task(
        process.RelabelledHcalSimHits
    )
    process.schedule.associate(process.HcalSimHitTask)
    return process

def setupSimHitTables(process, subdet):
    if subdet in merging_params:
        params = merging_params[subdet]
    else:
        valid_options = "', '".join(merging_params.keys())
        raise ValueError(f"subdet must be one of '{valid_options}'")

    if subdet == 'HGCAL':
        which_producer = 'HGCalSimHitPositionTableProducer'
    elif subdet == 'L1THGCAL':
        which_producer = 'L1THGCalSimHitPositionTableProducer'
    else:
        which_producer = 'CaloSimHitPositionTableProducer'

    setattr(process, 'SimHit%sTable'%subdet,
        cms.EDProducer(
            which_producer,
            src = cms.VInputTag(*params.simhits),
            name = cms.string('SimHits%s'%subdet),
            cut = cms.string(''),
            doc = cms.string('')
        )
    )

    setattr(process, 'SimHit%sTablesTask'%subdet,
        cms.Task(
            getattr(process, 'SimHit%sTable'%subdet)
        )
    )

    process.schedule.associate(
            getattr(process, 'SimHit%sTablesTask'%subdet)
    )

    return process
