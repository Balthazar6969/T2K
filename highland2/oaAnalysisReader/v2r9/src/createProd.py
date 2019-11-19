"""
"""
import os
import re
import sys
import ROOT as rt
from array import array
from collections import namedtuple, defaultdict

__author__ = 'Anselmo Cervera'
__email__ = 'acervera [at] ific.uv.es'

#######################################################
def process(prodName, fileName):
    
    """  comments
    """

    if os.path.exists(prodName):
        print "Folder ", prodName, " already exists. Please choose a different name "
        return
        
    print "Opening file ", fileName
    f = rt.TFile(fileName);

    print "Getting version ... "
    version = get_version(f)
    
    print "MakeProject(\"oaAnalysisReader\",\"*\",\"nocompilation+\") ..."
    f.MakeProject("oaAnalysisReader","*","nocompilation+")

    print "mv oaAnalysisReader ", prodName
    os.rename("oaAnalysisReader",prodName)

    print "rm -f MAKEP"
    os.remove(os.path.join(prodName,"MAKEP"))

    print "mv oaAnalysisReaderProjectHeaders.h oaAnalysisReaderProjectHeaders.h0"
    os.rename(os.path.join(prodName,"oaAnalysisReaderProjectHeaders.h"), os.path.join(prodName,"oaAnalysisReaderProjectHeaders.h0")) 

    
    print "create the copile_reader file"
    create_compile_reader(prodName)

    print "create Version.h"
    create_version(prodName, version)


#######################################################
def create_compile_reader(prodName):

    with open(os.path.join(prodName,"compile_reader"), 'w') as g:
        g.write("#!/bin/sh\n")
        g.write("# Make sure the environment is properly configured.\n")
        g.write("source setup.sh\n")
        g.write("\n")
        g.write("PRODUCTIONDIR=");g.write(prodName);g.write("\n")
        g.write("LIBS=`root-config --libs`\n")
        g.write("CC=`root-config --cxx` \n")
        g.write("CFLAGS=`root-config --cflags`\n")
        g.write("\n")
        g.write("# Get the system identifier.\n")
        g.write("cd $OAANALYSISREADERROOT/src/$PRODUCTIONDIR\n")
        g.write("\n")
        g.write("rm -f oaAnalysisReaderProjectDict.*\n")
        g.write("cp -f oaAnalysisReaderProjectHeaders.h0 oaAnalysisReaderProjectHeaders.h\n")
        g.write("\n")
        g.write("echo \"GENERATING DICTIONARY\"\n")
        g.write("rootcint -f oaAnalysisReaderProjectDict.cxx -c -I$ROOTSYS/include/root  -I\"$ROOTSYS/include/root\"  oaAnalysisReaderProjectHeaders.h oaAnalysisReaderLinkDef.h \n")
        g.write("\n")
        g.write("echo \"COMPILING\"\n")
        g.write("$CC -c -O2 -fPIC $CFLAGS  -I$OAANALYSISREADERROOT/src/$PRODUCTIONDIR oaAnalysisReaderProjectSource.cxx -o $OAANALYSISREADERROOT/$OAANALYSISREADERCONFIG/oaAnalysisReaderProjectSource.o\n")
        g.write("\n")
        g.write("echo \"LINKING\"\n")
        g.write("LIBNAME=liboaAnalysisReader.so\n")
        g.write("if [ $CMTCONFIG == Darwin ]; then\n")
        g.write("    LIBNAME=liboaAnalysisReader.dylib\n")
        g.write("fi\n")
        g.write("\n")
        g.write("$CC $OAANALYSISREADERROOT/$OAANALYSISREADERCONFIG/oaAnalysisReaderProjectSource.o  -dynamiclib -shared $LIBS  -o $OAANALYSISREADERROOT/$OAANALYSISREADERCONFIG/$LIBNAME\n")
        g.write("cp ND__*.h $OAANALYSISREADERROOT/$OAANALYSISREADERCONFIG/\n")
        g.write("\n")
        g.write("echo \"ADDING VERSION\"\n")
        g.write("cat oaAnalysisReaderProjectHeaders.h0 Version.h > oaAnalysisReaderProjectHeaders.h\n")

#######################################################
def create_version(prodName, version):

    pv = parse_version(version)
    
    with open(os.path.join(prodName,"Version.h"), 'w') as g:
        g.write("#ifndef ANATOOLS_FILE_VERSION\n")
        g.write("#define ANATOOLS_FILE_VERSION "); g.write(version); g.write("\n")
        g.write("#define ANATOOLS_FILE_MAJOR_VERSION ");g.write(pv[0]);g.write("\n")
        g.write("#define ANATOOLS_FILE_MINOR_VERSION ");g.write(pv[1]);g.write("\n")
        g.write("#define ANATOOLS_FILE_PATCH_VERSION ");g.write(pv[2]);g.write("\n")
        g.write("#define BEFORE_ANATOOLS_FILE(v,r,p) ((ANATOOLS_FILE_MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_VERSION==(r)&&ANATOOLS_FILE_PATCH_VERSION<(p))||(ANATOOLS_FILE_MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_VERSION<(r))||(ANATOOLS_FILE_MAJOR_VERSION<(v)))\n")
        g.write("#define EQUALS_ANATOOLS_FILE(v,r,p) (ANATOOLS_FILE_MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_VERSION==(r)&&ANATOOLS_FILE_PATCH_VERSION==(p))\n")
        g.write("#endif\n")

#######################################################
def parse_version(version):

    pv = ["","",""]

    pv[0] = (version.partition('v')[2]).partition('r')[0]
    pv[1] = (version.partition('r')[2]).partition('p')[0]
    pv[2] = (version.partition('p')[2]).partition('\n')[0]

    if pv[2] == '':
        pv[2]='0'
    
    print "Parsed version",  pv
    

    return pv
        
#######################################################
def get_version(f):


  tree = f.Get("HeaderDir/BasicHeader")
#  if tree:
#      print "ERROR: input file doesn't contain HeadeDir/BasicHeader tree!"
#      sys.exit(1)

  for entry in tree:      
      print "version", entry.SoftwareVersion
      # need this to remove special characters (^@)
      clean_version = re.sub('\W+','', entry.SoftwareVersion )
      return clean_version


#######################################################        
def usage():
    print "USAGE: python createProd.py <production_name> <root_file>"


#######################################################
if __name__ == '__main__':

    if (len(sys.argv) !=3):
        usage()
        sys.exit(0)
    
    process(sys.argv[1],sys.argv[2])

