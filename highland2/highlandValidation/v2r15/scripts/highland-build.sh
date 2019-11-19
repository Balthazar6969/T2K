#!/bin/env bash

# cd into Jenkins job's workspace
cd $WORKSPACE

# cp the cmt project so CMT will find this workspace
cp -au ~/software/ND280_dev/cmt ./

cd $WORKSPACE/highland2/highlandValidation/cmt
source setup.sh

# set the appropriate production
echo '============================================='
if echo "$WORKSPACE" | grep -q "p5"; then
    echo 'setting prod5F in oaAnalysisReader'
    sed -i -r "s/prod6B/prod5F/" $WORKSPACE/highland2/oaAnalysisReader/cmt/requirements
else
    echo 'stting prod6B in oaAnalysisReader'
    sed -i -r "s/prod5F/prod6B/" $WORKSPACE/highland2/oaAnalysisReader/cmt/requirements
fi

# setup some of the python utils
cd ..
rm -fr src/Python
source src/setup_pyutils.sh

# perform a clean build
cd $WORKSPACE/highland2/highlandValidation/cmt
cmt -without_version_directory br cmt config
source setup.sh
# make clean only highland2/psyche packages
cmt -without_version_directory br 'if grep -Eq "package (highland2|psyche)" requirements; then make binclean; fi' 
cmt -without_version_directory br 'if grep -Eq "package (highland2|psyche)" requirements; then make clean; fi'
# for some reason the rootcint dictionary doesn't get cleaned up and causes builds to fail. Manually rm for now
cmt -without_version_directory br 'if [ -d ../dict/ ]; then rm -rf ../dict/; fi'
cmt -without_version_directory br 'if grep -Eq "package (highland2|psyche)" requirements; then make; fi'
