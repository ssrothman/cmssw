import FWCore.ParameterSet.Config as cms

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
        )
    ),

    #ibid
    'L1THGCAL' : cms.PSet(
        caloR = cms.double(136.5),
        caloZ = cms.double(318.5),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag('TCSimHits')
    ),

    #geometry from https://cms-docdb.cern.ch/cgi-bin/DocDB/RetrieveFile?docid=3661&filename=quarterView-HBHE.pdf&version=1
    'HCAL' : cms.PSet(
        caloR = cms.double(180.6),
        caloZ = cms.double(388.8),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'g4SimHits:HcalHits',
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
        )
    ),

    #ibid
    'ECALHCAL' : cms.PSet(
        caloR = cms.double(135.0),
        caloZ = cms.double(310.0),
        overlapThreshold = cms.double(0.2),
        distanceTol = cms.double(0.1),
        simhits = cms.VInputTag(
            'g4SimHits:EcalHitsEB',
            'g4SimHits:HcalHits',
        )
    )
}


