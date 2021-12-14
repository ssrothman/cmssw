#!/bin/sh

function die { echo $1: status $2 ;  exit $2; }

cd $CMSSW_BASE/src
git clone git@github.com:ssrothman/DRNData.git
mv DRNData/models $CMSSW_BASE/src/RecoEcal/EgammaClusterProducers/data/