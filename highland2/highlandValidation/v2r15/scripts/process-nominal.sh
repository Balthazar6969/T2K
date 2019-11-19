#!/bin/env bash

# cd into Jenkins job's workspace
cd $WORKSPACE

# use anoncvs to checkout
export CVSROOT=:ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository

# remove the validation folder 
echo '============================================='
echo 'rm -fr highland2/highlandValidation'
rm -fr highland2/highlandValidation/cmt
rm -fr highland2/highlandValidation/CVS
rm -fr highland2/highlandValidation/doc
rm -fr highland2/highlandValidation/parameters
rm -fr highland2/highlandValidation/scripts
rm -fr highland2/highlandValidation/src
rm -fr highland2/highlandValidation/validator
rm -fr highland2/highlandValidation/data

# checkout highland2/highlandValidation HEAD, we want all outputs to
# be in the same place
echo '============================================='
echo 'cvs co highland2/highlandValidation'
cvs checkout highland2/highlandValidation

echo '============================================='
echo 'cd NOMINAL'
mkdir NOMINAL
cd NOMINAL

# remove the validation folder 
echo '============================================='
echo 'rm -fr highland2/highlandValidation'
rm -fr highland2/highlandValidation/cmt
rm -fr highland2/highlandValidation/CVS
rm -fr highland2/highlandValidation/doc
rm -fr highland2/highlandValidation/parameters
rm -fr highland2/highlandValidation/scripts
rm -fr highland2/highlandValidation/src
rm -fr highland2/highlandValidation/validator
rm -fr highland2/highlandValidation/data
    
# checkout highland2/highlandValidation HEAD, since we want to use the latest validation scripts and instructions
# we will later move the results of the validation to the other validation folder (above)
echo '============================================='
echo 'cvs co highland2/highlandValidation'
cvs checkout highland2/highlandValidation

# remove psyche and highland2 directories if they exist (from previous
# HEAD processing) else the validation will pickup the HEAD
# versions. This also serves to clean up the validation workspace and
# ensure that we don't have leftover output files
#echo '============================================='
#echo 'removing all code under NOMINAL'
#rm -rf nd280Highland2/
#rm -rf nd280Psyche/
#rm -rf psyche/
#rm -rf highland2/

# checkout frozen nd280Highland2 
echo '============================================='
echo 'cmt co -R nd280Highland2'
cmt co -R nd280Highland2 -r `grep JENKINS_PROCESS_NOMINAL_ND280HIGHLAND2 highland2/highlandValidation/validator/*.py | grep -E -o 'v[0-9]+r[0-9]+(p[0-9]+)?'`

# config
echo '============================================='
echo 'cd highland2/highlandValidation/cmt'
cd highland2/highlandValidation/cmt

export CMTPATH=$WORKSPACE/NOMINAL:$WORKSPACE:$CMTPATH
echo '============================================='
echo 'CMTPATH=' $CMTPATH

echo '============================================='
echo 'cmt show use'
cmt -without_version_directory show uses

echo '============================================='
echo 'cmt config'
cmt -without_version_directory config

echo '============================================='
echo 'source setup.sh'
source setup.sh

echo '============================================='
echo 'setup the python utils'
cd ../src
rm -fr Python
source setup_pyutils.sh
cd ../cmt

# build
echo '============================================='
echo 'cmt broadcast make'
cmt br cmt make


# temporary. there is a problem with the ECAL likelihood below v2r13. Only used for prod5
echo '============================================='
if echo "$WORKSPACE" | grep -q "p5"; then
    echo 'setting UseReconDirTrackerECal = 0'
    sed -i -r "s/UseReconDirTrackerECal = 1/UseReconDirTrackerECal = 0/" $WORKSPACE/NOMINAL/highland2/highlandValidation/parameters/*.parameters.dat
else
    echo 'setting UseReconDirTrackerECal = 1'
    sed -i -r "s/UseReconDirTrackerECal = 0/UseReconDirTrackerECal = 1/" $WORKSPACE/NOMINAL/highland2/highlandValidation/parameters/*.parameters.dat
fi


# process
cd ../validator
sed -i -r "s/PROCESS_CODE( *)?=( *)?[0-9]+/PROCESS_CODE = 0/" config.py

echo '============================================='
echo 'python process.py'
python process.py


cd ../validator
sed -i -r "s/PROCESS_CODE( *)?=( *)?[0-9]+/PROCESS_CODE = 3/" config.py

echo '============================================='
echo 'python process.py'
python process.py


echo '============================================='
echo 'mkdir $WORKSPACE/highland2/highlandValidation/outputs'
mkdir $WORKSPACE/highland2/highlandValidation/outputs

echo '============================================='
echo 'copy outputs to main highlandValidation'
echo 'cp -rf $WORKSPACE/NOMINAL/highland2/highlandValidation/outputs/0 $WORKSPACE/highland2/highlandValidation/outputs/.'
cp -rf $WORKSPACE/NOMINAL/highland2/highlandValidation/outputs/0 $WORKSPACE/highland2/highlandValidation/outputs/.
echo 'cp -rf $WORKSPACE/NOMINAL/highland2/highlandValidation/outputs/3 $WORKSPACE/highland2/highlandValidation/outputs/.'
cp -rf $WORKSPACE/NOMINAL/highland2/highlandValidation/outputs/3 $WORKSPACE/highland2/highlandValidation/outputs/.
