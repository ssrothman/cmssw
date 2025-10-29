#include "PhysicsTools/NanoAOD/interface/SimpleFlatTableProducer.h"

#include "DataFormats/L1THGCal/interface/HGCalTriggerCell.h"
typedef SimpleFlatTableProducer<l1t::HGCalTriggerCell> SimpleHGCalTriggerCellFlatTableProducer;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SimpleHGCalTriggerCellFlatTableProducer);

#include "DataFormats/L1THGCal/interface/HGCalModule.h"
typedef SimpleFlatTableProducer<l1t::HGCalModule> SimpleHGCalModuleFlatTableProducer;
DEFINE_FWK_MODULE(SimpleHGCalModuleFlatTableProducer);
