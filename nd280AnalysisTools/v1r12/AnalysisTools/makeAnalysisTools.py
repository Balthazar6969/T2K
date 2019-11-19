#!/usr/bin/env python

import string
import subprocess
import os, sys
import glob

if not os.getenv("ROOTSYS"):
    print "Cannot find ROOTSYS environment variable! Run ... source relevant thisroot.(c)sh ... first!"
    sys.exit(1)

command = ""
UsePython = True
UseCMT = True
IgnoreVersionInfo = False

try:
    import ROOT
except ImportError:
    print "Failed to load pyROOT, falling back to compileAnalysisTools.C"
    UsePython = False

CheckCMTCommand = ['cmt', '--help']

print "[INFO]: Checking for CMT"
try:
  HashCMTRtnCode = subprocess.call(CheckCMTCommand, shell=True,
    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  print "[INFO]: Are using CMT"
except:
  print "[INFO]: Are not using CMT"
  UseCMT = False

sys.stdout.flush()

if UsePython:
    if UseCMT:
      IgnoreVersionCheckCommand = ['cmt', 'show', 'macro_value', 'nd280AnalysisTools_IgnoreVersionInfo']
      proc = subprocess.Popen(IgnoreVersionCheckCommand,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
      proc.wait()
      IgnoreVersionInfoMacro = proc.stdout.read().rstrip()
      if IgnoreVersionInfoMacro == "true":
        IgnoreVersionInfo = True
        print "[INFO]: Ignoring version info for this build."
      else:
        print "[INFO]: Require version info for this build. (nd280AnalysisTools_IgnoreVersionInfo: %s)" % IgnoreVersionInfoMacro

    command = ['python','runAnalyses.py','-c', '--system-compiler']

    if IgnoreVersionInfo:
      command.append("--ignore-version-info")

    file = open('input-file.list','r')
    line = file.readline()
    line = string.strip(line)
    command.append(line)
else:
    command = ['root','-l','-b','compileAnalysisTools.C']

sys.stdout.flush()
buildRtnCode = subprocess.call(command)

if not UsePython: # Use Root
  command = ['mkdir', '-p', os.getenv("ND280ANALYSISTOOLSROOT") + "/"
  + os.getenv("ND280ANALYSISTOOLSCONFIG") + "/AnalysisTools"]
  mk = subprocess.call(command)
  files = glob.glob("*.so")
  files.extend(glob.glob("*.d"))
  files.append("libReadoaAnalysis")
  command = ['mv']
  command.extend(files)
  command.append(os.getenv("ND280ANALYSISTOOLSROOT") + "/"
    + os.getenv("ND280ANALYSISTOOLSCONFIG") + "/AnalysisTools/")
  mv = subprocess.call(command)

if not buildRtnCode:
  stamp = os.getenv("ND280ANALYSISTOOLSROOT") + "/" \
    + os.getenv("ND280ANALYSISTOOLSCONFIG") + "/nd280AnalysisTools.stamp"
  command = ['touch', stamp]

  if subprocess.call(command):
    print "[WARN]: Failed to touch " + stamp

  # Check whether we've already filled the ANATOOLS_FILE_VERSION info
  HeaderFile = os.getenv("ND280ANALYSISTOOLSROOT") + "/" \
    + os.getenv("ND280ANALYSISTOOLSCONFIG") \
    + "/AnalysisTools/libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
  header = open(HeaderFile, 'r')
  filled = False

  for line in header:
    if line.find("ANATOOLS_FILE") != -1:
      print "[INFO]: Skipping filling ANATOOLS_FILE info, as already set"
      filled = True
      break

  if (not IgnoreVersionInfo) and (not filled):
    command = ['root','-l','-b','exportVersion.C()']
    if subprocess.call(command):
      print "[ERROR]: Failed to add versioning info."
      sys.exit(1)
    else:
      print "[INFO]: Added versioning info to %s" % HeaderFile

sys.exit(buildRtnCode)
