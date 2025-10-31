import FWCore.ParameterSet.Config as cms # pyright: ignore[reportMissingImports]

merging_params = {
    #geometry from https://cms-docdb.cern.ch/cgi-bin/PublicDocDB/RetrieveFile?docid=13251&filename=20210803%20HGCAL%20PARAMETER%20DRAWING.pdf&version=11
    'HGCAL' : cms.PSet(
        caloR = cms.double(136.5),
        caloZ = cms.double(318.5),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'g4SimHits:HGCHitsEE',
            'g4SimHits:HGCHitsHEfront',
            'g4SimHits:HGCHitsHEback'
        ),
        rechits = cms.VInputTag(
            'HGCalRecHit:HGCEERecHits',
            'HGCalRecHit:HGCEBRecHits',
            'HGCalRecHit:HGCHEFRecHits',
            #'HGCalRecHit:HGCHFNoseRecHits',

            #or
            #'particleFlowRecHitHGC'

            #or
            #'particleFlowRecHitHGC:Cleaned'
        ),
    ),

    #ibid
    'L1THGCAL' : cms.PSet(
        caloR = cms.double(136.5),
        caloZ = cms.double(318.5),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'TCSimHits'
        ),
        rechits = cms.VInputTag(
            'FloatingpointThreshold0'
        )
    ),

    #geometry from https://cms-docdb.cern.ch/cgi-bin/DocDB/RetrieveFile?docid=3661&filename=quarterView-HBHE.pdf&version=1
    'HCAL' : cms.PSet(
        caloR = cms.double(180.6),
        caloZ = cms.double(388.8),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'RelabelledHcalSimHits'
        ),
        rechits = cms.VInputTag(
            'hbhereco',
            'hfreco',
            'horeco',

            #or
            #'reducedHcalRecHits:hbhereco',
            #'reducedHcalRecHits:hfreco',
            #'reducedHcalRecHits:horeco'

            #or
            #'particleFlowRecHitHBHE',
            #'particleFlowRecHitHF',
            #'particleFlowRecHitHO'

            #or
            #'particleFlowRecHitHBHE:Cleaned',
            #'particleFlowRecHitHF:Cleaned',
            #'particleFlowRecHitHO:Cleaned'
        )
    ),

    #geometry is a guess. would love to find a real diagram
    'ECAL' : cms.PSet(
        caloR = cms.double(135.0),
        caloZ = cms.double(310.0),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'g4SimHits:EcalHitsEB',
        ),
        rechits = cms.VInputTag(
            'ecalRecHit:EcalRecHitsEB',
            'ecalRecHit:EcalRecHitsEE',
            'ecalPreshowerRecHit:EcalRecHitsES',

            #or
            #'reducedEcalRecHitsEB',
            #'reducedEcalRecHitsEE',
            #'ecalPreshowerRecHit:EcalRecHitsES',

            #or
            #'particleFlowRecHitECAL',
        )
    ),

    #ibid
}

merging_params['ECALHCAL'] = cms.PSet(
    caloR = merging_params['ECAL'].caloR,
    caloZ = merging_params['ECAL'].caloZ,

    overlapThreshold = cms.double(0.2),
    distanceTol = cms.double(0.1),

    simhits = cms.VInputTag(
        merging_params['ECAL'].simhits +
        merging_params['HCAL'].simhits
    ),
    rechits = cms.VInputTag(
        merging_params['ECAL'].rechits +
        merging_params['HCAL'].rechits
    )
)


