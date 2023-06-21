#!/bin/bash

cmsDriver.py Configuration/Generator/python/DoubleGammaPt35Extended_pythia8_cfi.py -s GEN,SIM --conditions auto:phase1_2022_realistic --datatier 'GEN-SIM'  --eventcontent FEVTDEBUG --era Run3 --customise DPGAnalysis/PFNanoAOD/pfNano_cff.customizeOutputForPF -n 10 --mc --fileout file:step1.root --no_exec --python_filename step1_DoublePhotonPt35_Run3.py
