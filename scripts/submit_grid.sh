#!/bin/bash

if [ -z $1 ]
then
    echo "Suffix required!"
    #exit
fi
SUFFIX=$1

if [ -z $2 ]
then
    echo "Config file needed!"
    #exit
fi
CONFIG=$2

NAME="ywng"

PRUN="prun --optGridOutputSampleName=user.${NAME}.%in:name[1]%.%in:name[2]%.%in:name[3]%.${SUFFIX}"
#PRUN="prun --optGridOutputSampleName=user.%nickname%.%in:name[3]%.${SUFFIX}"

if [ ! -z $3 ]
then
    DESTSITE=$3
    echo "Submitting to ${DESTSITE}"
    PRUN="${PRUN} --optGridDestSE=${DESTSITE}"
fi

# data15
#../../xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/data15.EXOT18.list        --inputList --inputRucio --submitDir ${SUFFIX}_data15 --force        ${PRUN}

# data16
#../../xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/data16.EXOT18.list        --inputList --inputRucio --submitDir ${SUFFIX}_data16 --force        ${PRUN}

# jetjet
#../../xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/jetjet.EXOT18.list        --inputList --inputRucio --submitDir ${SUFFIX}_jetjet --force --isMC ${PRUN}

# WHad
#../../xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/WHad.EXOT18.list          --inputList --inputRucio --submitDir ${SUFFIX}_WHad   --force --isMC ${PRUN}

# ZHad
#./xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/ZHad.EXOT18.list          --inputList --inputRucio --submitDir ${SUFFIX}_ZHad   --force --isMC ${PRUN}

# ISRjet signal
../../xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files /lustre/SCRATCH/atlas/ywng/optimization_bkgnd/selection-laser/dijetISR/etc/signal.ISRphoton.EXOT18.list --inputList --inputRucio --submitDir ${SUFFIX}_a --force --isMC ${PRUN} 


# ttbar
#./xAODAnaHelpers/scripts/xAH_run.py --config ${CONFIG} --files dijetISR/etc/ttbar.JETM6.list  --inputList --inputRucio --submitDir ${SUFFIX}_ttbar  --force --isMC ${PRUN}
