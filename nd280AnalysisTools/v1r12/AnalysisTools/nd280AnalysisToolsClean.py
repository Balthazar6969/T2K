#! /usr/bin/env python

import os, sys
import re

so_libs = re.compile('.*\.so')
d_libs = re.compile('.*\.d$')

curr_dir = os.getcwd()

nd280analysistoolsdir = os.getenv("ND280ANALYSISTOOLSROOT")

if not nd280analysistoolsdir:
    print "Cannot find ND280ANALYSISTOOLSROOT environment variable! Run ... source setup.(c)sh ... first!"
    sys.exit(1)

nd280analysistoolsdirs = [nd280analysistoolsdir + "/" + os.getenv("ND280ANALYSISTOOLSCONFIG") + "/AnalysisTools", nd280analysistoolsdir + "/AnalysisTools"]

for nd280analysistoolsdir in nd280analysistoolsdirs:
  if os.path.exists(nd280analysistoolsdir):
    files = os.listdir(nd280analysistoolsdir)

    os.chdir(nd280analysistoolsdir)

    for file in files:
        if so_libs.search(file) or d_libs.search(file) :
            os.remove(file)

    if os.path.exists("libReadoaAnalysis"):
        files = os.listdir("libReadoaAnalysis")
        os.chdir("libReadoaAnalysis")
        for file in files:
          # print file
            os.remove(file)
        os.chdir('..')
        os.rmdir("libReadoaAnalysis")

  os.chdir(curr_dir)
