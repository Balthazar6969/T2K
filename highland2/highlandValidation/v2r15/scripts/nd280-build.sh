#!/bin/env bash

if [ "$#" -eq 1 ]; then
    # cd into Jenkins job's workspace
    cd $WORKSPACE

    # cp the cmt project so CMT will find this workspace
    cp -au ~/software/ND280_dev/cmt ./

    # setup input-file file
    echo "$1" > nd280AnalysisTools/AnalysisTools/input-file.list

    # build
    cd nd280AnalysisTools/cmt
    cmt -without_version_directory config
    cmt -without_version_directory make
else
    echo "run with ./$0 path/to/oa/file"
    exit 1
fi
