#!/bin/env bash
# @author: Tianlu Yuan
# @email: tianlu.yuan@colorado.edu

validate () {
    cd ${HIGHLANDVALIDATIONROOT}/validator || exit 1
    # remove plots artifacts that may exist from previous build
    echo "rm -rf ../outputs/$1/plots/*"
    rm -rf ../outputs/$1/plots/*
    nosetests --with-xunit --xunit-file="nosetests_$1.xml"
}

# cd into Jenkins job's workspace
cd $WORKSPACE

# config
cd highland2/highlandValidation/cmt
cmt config
source setup.sh

# cleanup python utils
cd ..
rm -fr src/Python
source src/setup_pyutils.sh
cd $WORKSPACE/highland2/highlandValidation/cmt

# set the config.py process_code which determines the type of
# comparisons to be made for validation
cd ${HIGHLANDVALIDATIONROOT}/validator || exit 1
sed -i -r "s/PROCESS_CODE( *)?=( *)?[0-9]+/PROCESS_CODE = $1/" config.py

# process and validate
if [[ $2 == "both" ]]; then
    echo "*********************************************************************************************"
    echo "Running both processing and validation..."
    echo "*********************************************************************************************"
    python process.py
    validate $1
elif [[ $2 == "process" ]]; then
    echo "*********************************************************************************************"
    echo "Running processing only..."
    echo "*********************************************************************************************"
    python process.py
elif [[ $2 == "validate" ]]; then
    echo "*********************************************************************************************"
    echo "Running validation only..."
    echo "*********************************************************************************************"
    validate $1
else
    echo "Unknown option $2"
fi

exit 0
