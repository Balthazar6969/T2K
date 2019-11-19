CMTPATH=/mnt/data/backup/home/cfrancois/T2K2:/mnt/data/T2K/frozen
CMT_tag=$(tag)
CMTROOT=/mnt/data/T2K/CMT/v1r20p20081118
CMT_root=/mnt/data/T2K/CMT/v1r20p20081118
CMTVERSION=v1r20p20081118
cmt_hardware_query_command=uname -m
cmt_hardware=`$(cmt_hardware_query_command)`
cmt_system_version_query_command=${CMTROOT}/mgr/cmt_linux_version.sh | ${CMTROOT}/mgr/cmt_filter_version.sh
cmt_system_version=`$(cmt_system_version_query_command)`
cmt_compiler_version_query_command=${CMTROOT}/mgr/cmt_gcc_version.sh | ${CMTROOT}/mgr/cmt_filter3_version.sh
cmt_compiler_version=`$(cmt_compiler_version_query_command)`
PATH=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandTools/v2r17p1/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCore/v2r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheCore/v3r21/Linux-x86_64:/mnt/data/T2K/frozen/ROOT/v5r34p09n04/Linux-x86_64/bin:/mnt/data/T2K/frozen/GSL/v1r15p0n00/Linux-x86_64/bin:/mnt/data/T2K/frozen/MYSQL/v5r051an01/Linux-x86_64/bin:/mnt/data/T2K/frozen/EXTERN/v3r7/scripts:/mnt/data/backup/home/cfrancois/T2K2/psyche/psychePolicy/v3r1/scripts:/mnt/data/T2K/CMT/v1r20p20081118/${CMTBIN}:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCInc4PiAnalysis/v0r0/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCIncFwdAnalysis/v0r0/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCAnalysis/v2r3/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/numuCCAnalysis/v2r13/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/baseTrackerAnalysis/v2r11/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/baseAnalysis/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandIO/v2r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCorrections/v2r7/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheSystematics/v3r23/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheSelections/v3r25/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandUtils/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandEventModel/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/oaAnalysisReader/v2r9/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheIO/v3r17/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheND280Utils/v3r23/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheUtils/v3r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheEventModel/v3r21/Linux-x86_64:/mnt/data/T2K/frozen/oaControlSample/v1r1p1/scripts:/mnt/data/T2K/frozen/oaControlSample/v1r1p1/Linux-x86_64:/mnt/data/T2K/frozen/ecalTestBeam/v0r5/Linux-x86_64:/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/AnalysisTools/libReadoaAnalysis:/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/AnalysisTools:/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/Linux-x86_64:/mnt/data/T2K/frozen/oaAnalysis/v5r31p9/Linux-x86_64:/mnt/data/T2K/frozen/eventDisplay/v3r63/Linux-x86_64:/mnt/data/T2K/frozen/oaAlignTools/v0r7/Linux-x86_64:/mnt/data/T2K/frozen/soffTasks/v1r23/Linux-x86_64:/mnt/data/T2K/frozen/ecalCalib/v2r11/Linux-x86_64:/mnt/data/T2K/frozen/mppcCalib/v1r17/Linux-x86_64:/mnt/data/T2K/frozen/tfbCalib/v1r37/Linux-x86_64:/mnt/data/T2K/frozen/nd280/v11r37/Linux-x86_64:/mnt/data/T2K/frozen/oaRecon/v5r3p3/Linux-x86_64:/mnt/data/T2K/frozen/p0decalRecon/v2r7p1/Linux-x86_64:/mnt/data/T2K/frozen/ecalRecon/v10r23p5/Linux-x86_64:/mnt/data/T2K/frozen/trackerRecon/v3r3p1/Linux-x86_64:/mnt/data/T2K/frozen/fgdRecon/v6r3p3/Linux-x86_64:/mnt/data/T2K/frozen/trexRecon/v2r27p5/Linux-x86_64:/mnt/data/T2K/frozen/tpcRecon/v6r29/Linux-x86_64:/mnt/data/T2K/frozen/p0dRecon/v9r1p3/Linux-x86_64:/mnt/data/T2K/frozen/sbcat/v5r3/Linux-x86_64:/mnt/data/T2K/frozen/reconUtils/v1r27/Linux-x86_64:/mnt/data/T2K/frozen/elecSim/v6r23p3/Linux-x86_64:/mnt/data/T2K/frozen/fgdCalib/v0r7/app/:/mnt/data/T2K/frozen/fgdCalib/v0r7/Linux-x86_64:/mnt/data/T2K/frozen/smrdCalib/v1r27p3/Linux-x86_64:/mnt/data/T2K/frozen/oaCalib/v4r7/Linux-x86_64:/mnt/data/T2K/frozen/oaBeamData/v0r25/Linux-x86_64:/mnt/data/T2K/frozen/oaDataQuality/v1r21/src:/mnt/data/T2K/frozen/oaDataQuality/v1r21/Linux-x86_64:/mnt/data/T2K/frozen/tfbApplyCalib/v3r45/Linux-x86_64:/mnt/data/T2K/frozen/fgdRawData/v2r3/Linux-x86_64:/mnt/data/T2K/frozen/oaUnpack/v3r3/Linux-x86_64:/mnt/data/T2K/frozen/oaSandSimulation/v0r3p1/Linux-x86_64:/mnt/data/T2K/frozen/atmPitSim/v1r1/Linux-x86_64:/mnt/data/T2K/frozen/oaCosmicTrigger/v1r5/Linux-x86_64:/mnt/data/T2K/frozen/oaCherryPicker/v0r11/Linux-x86_64:/mnt/data/T2K/frozen/nd280mc/v5r3p5/Linux-x86_64:/mnt/data/T2K/frozen/oaApplyAlign/v2r1/Linux-x86_64:/mnt/data/T2K/frozen/oaGeometryLookup/v0r1/Linux-x86_64:/mnt/data/T2K/frozen/oaGeomInfo/v5r3/Linux-x86_64:/mnt/data/T2K/frozen/oaChanInfo/v3r5/Linux-x86_64:/mnt/data/T2K/frozen/ingridUtils/v1r3p1/Linux-x86_64:/mnt/data/T2K/frozen/nd280Control/v1r67/app:/mnt/data/T2K/frozen/oaUtility/v5r9/Linux-x86_64:/mnt/data/T2K/frozen/magnetCalib/v4r5/Linux-x86_64:/mnt/data/T2K/frozen/calibTables/v1r23/Linux-x86_64:/mnt/data/T2K/frozen/oaOfflineDatabase/v2r7/Linux-x86_64:/mnt/data/T2K/frozen/oaSlowControlDatabase/v1r11/Linux-x86_64:/mnt/data/T2K/frozen/oaRuntimeParameters/v0r5/Linux-x86_64:/mnt/data/T2K/frozen/oaRawEvent/v3r25/Linux-x86_64:/mnt/data/T2K/frozen/oaEvent/v8r9p19/Linux-x86_64:/mnt/data/T2K/frozen/testBase/v1r15/scripts:/mnt/data/T2K/frozen/testBase/v1r15/Linux-x86_64:/mnt/data/T2K/frozen/nd280Policy/v2r39/scripts:/mnt/data/T2K/frozen/GEANT/v9r4p04n00/:/mnt/data/T2K/frozen/CLHEP/v2r1p1n00/Linux-x86_64/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/mnt/data/home/cfrancois/bin:/mnt/data/T2K/frozen/oaEvent/v8r9p19/scripts
CLASSPATH=/mnt/data/T2K/CMT/v1r20p20081118/java
debug_option=-g
cc=gcc
ccomp=$(cc) -c $(includes) $(cdebugflags) $(cflags) $(pp_cflags)
clink=$(cc) $(clinkflags) $(cdebugflags)
ppcmd=-I
preproc=c++ -MD -c 
cpp=g++
cppflags=-g -pipe -Wall -ansi -Wctor-dtor-privacy -Wnon-virtual-dtor -fpic -Woverloaded-virtual 
pp_cppflags=-D_GNU_SOURCE
cppcomp=$(cpp) -c $(includes) $(cppdebugflags) $(cppflags) $(pp_cppflags)
cpplinkflags=-Wl,--no-as-needed
cpplink=$(cpp) $(cpplinkflags) $(cppdebugflags)
for=g77
fflags=$(debug_option)
fcomp=$(for) -c $(fincludes) $(fflags) $(pp_fflags)
flink=$(for) $(flinkflags)
javacomp=javac -classpath $(src):$(CLASSPATH) 
javacopy=cp
jar=jar
X11_cflags=-I/usr/include
Xm_cflags=-I/usr/include
X_linkopts=-L/usr/X11R6/lib -lXm -lXt -lXext -lX11 -lm
lex=lex $(lexflags)
yaccflags= -l -d 
yacc=yacc $(yaccflags)
ar=ar cr
ranlib=ranlib
make_shlib=${CMTROOT}/mgr/cmt_make_shlib_common.sh extract
shlibsuffix=so
shlibbuilder=g++ $(cmt_installarea_linkopts) 
shlibflags=-shared
symlink=/bin/ln -fs 
symunlink=/bin/rm -f 
library_install_command=${symlink}
build_library_links=$(cmtexe) build library_links -tag=$(tags)
remove_library_links=$(cmtexe) remove library_links -tag=$(tags)
cmtexe=${CMTROOT}/${CMTBIN}/cmt.exe
build_prototype=$(cmtexe) build prototype
build_dependencies=$(cmtexe) -tag=$(tags) build dependencies
build_triggers=$(cmtexe) build triggers
implied_library_prefix=-l
SHELL=/bin/sh
q="
src=../src/
doc=../doc/
inc=../src/
mgr=../cmt/
application_suffix=.exe
library_prefix=lib
lock_command=lockfile 
unlock_command=rm -rf 
lock_name=cmt
lock_suffix=.lock
lock_file=${lock_name}${lock_suffix}
MAKEFLAGS= --no-print-directory 
gmake_hosts=lx1 rsplus lxtest as7 dxplus ax7 hp2 aleph hp1 hpplus papou1-fe atlas
make_hosts=virgo-control1 rio0a vmpc38a
everywhere=hosts
install_command=cp 
uninstall_command=/bin/rm -f 
cmt_installarea_command=ln -fs 
cmt_uninstallarea_command=/bin/rm -f 
cmt_install_area_command=$(cmt_installarea_command)
cmt_uninstall_area_command=$(cmt_uninstallarea_command)
cmt_install_action=$(CMTROOT)/mgr/cmt_install_action.sh
cmt_installdir_action=$(CMTROOT)/mgr/cmt_installdir_action.sh
cmt_uninstall_action=$(CMTROOT)/mgr/cmt_uninstall_action.sh
cmt_uninstalldir_action=$(CMTROOT)/mgr/cmt_uninstalldir_action.sh
mkdir=mkdir
cmt_cvs_protocol_level=v1r1
cmt_installarea_prefix=InstallArea
CMT_PATH_remove_regexp=/[^/]*/
CMT_PATH_remove_share_regexp=/share/
NEWCMTCONFIG=x86_64-slc610-gcc447
highlandTools_tag=$(tag)
HIGHLANDTOOLSROOT=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandTools/v2r17p1
highlandTools_root=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandTools/v2r17p1
HIGHLANDTOOLSVERSION=v2r17p1
highlandTools_cmtpath=/mnt/data/backup/home/cfrancois/T2K2
highlandTools_offset=highland2
highlandTools_project=cfrancois
highlandTools_project_release=T2K2
highlandCore_tag=$(tag)
HIGHLANDCOREROOT=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCore/v2r21
highlandCore_root=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCore/v2r21
HIGHLANDCOREVERSION=v2r21
highlandCore_cmtpath=/mnt/data/backup/home/cfrancois/T2K2
highlandCore_offset=highland2
highlandCore_project=cfrancois
highlandCore_project_release=T2K2
psycheCore_tag=$(tag)
PSYCHECOREROOT=/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheCore/v3r21
psycheCore_root=/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheCore/v3r21
PSYCHECOREVERSION=v3r21
psycheCore_cmtpath=/mnt/data/backup/home/cfrancois/T2K2
psycheCore_offset=psyche
psycheCore_project=cfrancois
psycheCore_project_release=T2K2
psychePolicy_tag=$(tag)
PSYCHEPOLICYROOT=/mnt/data/backup/home/cfrancois/T2K2/psyche/psychePolicy/v3r1
psychePolicy_root=/mnt/data/backup/home/cfrancois/T2K2/psyche/psychePolicy/v3r1
PSYCHEPOLICYVERSION=v3r1
psychePolicy_cmtpath=/mnt/data/backup/home/cfrancois/T2K2
psychePolicy_offset=psyche
psychePolicy_project=cfrancois
psychePolicy_project_release=T2K2
ROOT_tag=$(tag)
ROOTROOT=/mnt/data/T2K/frozen/ROOT/v5r34p09n04
ROOT_root=/mnt/data/T2K/frozen/ROOT/v5r34p09n04
ROOTVERSION=v5r34p09n04
ROOT_cmtpath=/mnt/data/T2K/frozen
ROOT_project=T2K
ROOT_project_release=frozen
EXTERN_tag=$(tag)
EXTERNROOT=/mnt/data/T2K/frozen/EXTERN/v3r7
EXTERN_root=/mnt/data/T2K/frozen/EXTERN/v3r7
EXTERNVERSION=v3r7
EXTERN_cmtpath=/mnt/data/T2K/frozen
EXTERN_project=T2K
EXTERN_project_release=frozen
MYSQL_tag=$(tag)
MYSQLROOT=/mnt/data/T2K/frozen/MYSQL/v5r051an01
MYSQL_root=/mnt/data/T2K/frozen/MYSQL/v5r051an01
MYSQLVERSION=v5r051an01
MYSQL_cmtpath=/mnt/data/T2K/frozen
MYSQL_project=T2K
MYSQL_project_release=frozen
LD_LIBRARY_PATH=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandTools/v2r17p1/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCore/v2r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheCore/v3r21/Linux-x86_64:/mnt/data/T2K/frozen/ROOT/v5r34p09n04/Linux-x86_64/lib/root:/mnt/data/T2K/frozen/GSL/v1r15p0n00/Linux-x86_64/lib:/mnt/data/T2K/frozen/MYSQL/v5r051an01/Linux-x86_64/lib/mysql:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCInc4PiAnalysis/v0r0/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCIncFwdAnalysis/v0r0/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/antiNumuCCAnalysis/v2r3/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/numuCCAnalysis/v2r13/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/baseTrackerAnalysis/v2r11/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/baseAnalysis/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandIO/v2r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandCorrections/v2r7/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheSystematics/v3r23/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheSelections/v3r25/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandUtils/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandEventModel/v2r19/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/highland2/oaAnalysisReader/v2r9/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheIO/v3r17/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheND280Utils/v3r23/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheUtils/v3r21/Linux-x86_64:/mnt/data/backup/home/cfrancois/T2K2/psyche/psycheEventModel/v3r21/Linux-x86_64:/mnt/data/T2K/frozen/oaControlSample/v1r1p1/Linux-x86_64:/mnt/data/T2K/frozen/ecalTestBeam/v0r5/Linux-x86_64:/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/Linux-x86_64/AnalysisTools/libReadoaAnalysis:/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/Linux-x86_64/AnalysisTools:/mnt/data/T2K/frozen/oaAnalysis/v5r31p9/Linux-x86_64:/mnt/data/T2K/frozen/eventDisplay/v3r63/Linux-x86_64:/mnt/data/T2K/frozen/oaAlignTools/v0r7/Linux-x86_64:/mnt/data/T2K/frozen/soffTasks/v1r23/Linux-x86_64:/mnt/data/T2K/frozen/ecalCalib/v2r11/Linux-x86_64:/mnt/data/T2K/frozen/mppcCalib/v1r17/Linux-x86_64:/mnt/data/T2K/frozen/tfbCalib/v1r37/Linux-x86_64:/mnt/data/T2K/frozen/ingridRecon/v2r1/Linux-x86_64:/mnt/data/T2K/frozen/oaRecon/v5r3p3/Linux-x86_64:/mnt/data/T2K/frozen/p0decalRecon/v2r7p1/Linux-x86_64:/mnt/data/T2K/frozen/ecalRecon/v10r23p5/Linux-x86_64:/mnt/data/T2K/frozen/trackerRecon/v3r3p1/Linux-x86_64:/mnt/data/T2K/frozen/smrdRecon/v5r11/Linux-x86_64:/mnt/data/T2K/frozen/fgdRecon/v6r3p3/Linux-x86_64:/mnt/data/T2K/frozen/trexRecon/v2r27p5/Linux-x86_64:/mnt/data/T2K/frozen/tpcRecon/v6r29/Linux-x86_64:/mnt/data/T2K/frozen/p0dRecon/v9r1p3/Linux-x86_64:/mnt/data/T2K/frozen/sbcat/v5r3/Linux-x86_64:/mnt/data/T2K/frozen/oaRecPack/v8r41p5/Linux-x86_64:/mnt/data/T2K/frozen/RECPACK/v4r15p3/Linux-x86_64:/mnt/data/T2K/frozen/reconUtils/v1r27/Linux-x86_64:/mnt/data/T2K/frozen/elecSim/v6r23p3/Linux-x86_64:/mnt/data/T2K/frozen/fgdCalib/v0r7/Linux-x86_64:/mnt/data/T2K/frozen/smrdCalib/v1r27p3/Linux-x86_64:/mnt/data/T2K/frozen/oaCalib/v4r7/Linux-x86_64:/mnt/data/T2K/frozen/oaBeamData/v0r25/Linux-x86_64:/mnt/data/T2K/frozen/oaDataQuality/v1r21/Linux-x86_64:/mnt/data/T2K/frozen/tfbApplyCalib/v3r45/Linux-x86_64:/mnt/data/T2K/frozen/fgdRawData/v2r3/Linux-x86_64:/mnt/data/T2K/frozen/oaUnpack/v3r3/Linux-x86_64:/mnt/data/T2K/frozen/oaSandSimulation/v0r3p1/Linux-x86_64:/mnt/data/T2K/frozen/atmPitSim/v1r1/Linux-x86_64:/mnt/data/T2K/frozen/oaCosmicTrigger/v1r5/Linux-x86_64:/mnt/data/T2K/frozen/nd280mc/v5r3p5/Linux-x86_64:/mnt/data/T2K/frozen/oaApplyAlign/v2r1/Linux-x86_64:/mnt/data/T2K/frozen/oaGeometryLookup/v0r1/Linux-x86_64:/mnt/data/T2K/frozen/oaGeomInfo/v5r3/Linux-x86_64:/mnt/data/T2K/frozen/oaChanInfo/v3r5/Linux-x86_64:/mnt/data/T2K/frozen/ingridUtils/v1r3p1/Linux-x86_64:/mnt/data/T2K/frozen/oaUtility/v5r9/Linux-x86_64:/mnt/data/T2K/frozen/magnetCalib/v4r5/Linux-x86_64:/mnt/data/T2K/frozen/calibTables/v1r23/Linux-x86_64:/mnt/data/T2K/frozen/oaOfflineDatabase/v2r7/Linux-x86_64:/mnt/data/T2K/frozen/oaSlowControlDatabase/v1r11/Linux-x86_64:/mnt/data/T2K/frozen/oaRuntimeParameters/v0r5/Linux-x86_64:/mnt/data/T2K/frozen/oaRawEvent/v3r25/Linux-x86_64:/mnt/data/T2K/frozen/oaEvent/v8r9p19/Linux-x86_64:/mnt/data/T2K/frozen/GEANT/v9r4p04n00/Linux-x86_64/Linux-g++:/mnt/data/T2K/frozen/GEANT/v9r4p04n00/Linux-x86_64/plists/Linux-g++:/mnt/data/T2K/frozen/CLHEP/v2r1p1n00/Linux-x86_64/lib
MYSQL_linkopts=`$(MYSQLROOT)/$(MYSQL_tag)/bin/mysql_config  --libs`
MYSQL_BASE_DIR=/mnt/data/T2K/frozen/MYSQL/v5r051an01/Linux-x86_64
MYSQL_LIB_DIR=${MYSQL_BASE_DIR}/lib/mysql
MYSQL_INCLUDE_DIR=${MYSQL_BASE_DIR}/include/mysql
MAKFLAGS=--no-print-directory
GSL_tag=$(tag)
GSLROOT=/mnt/data/T2K/frozen/GSL/v1r15p0n00
GSL_root=/mnt/data/T2K/frozen/GSL/v1r15p0n00
GSLVERSION=v1r15p0n00
GSL_cmtpath=/mnt/data/T2K/frozen
GSL_project=T2K
GSL_project_release=frozen
GSL_cppflags=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --cflags`
GSL_linkopts=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --libs`
GSL_BASE_DIR=/mnt/data/T2K/frozen/GSL/v1r15p0n00/Linux-x86_64
GSL_LIB_DIR=${GSL_BASE_DIR}/lib
GSL_INCLUDE_DIR=${GSL_BASE_DIR}/include
ROOT_PATH=$(ROOTROOT)/$(tag)
ROOT_BIN=$(ROOT_PATH)/bin
ROOT_LIB=$(ROOT_PATH)/lib/root
ROOTSYS=/mnt/data/T2K/frozen/ROOT/v5r34p09n04/Linux-x86_64
PYTHONPATH=/mnt/data/T2K/frozen/nd280AnalysisTools/v1r9p3/macros:/mnt/data/T2K/frozen/soffTasks/v1r23/Linux-x86_64:/mnt/data/T2K/frozen/reconUtils/v1r27/scripts:/mnt/data/T2K/frozen/reconUtils/v1r27/macros:/mnt/data/T2K/frozen/oaDataQuality/v1r21/src:/mnt/data/T2K/frozen/nd280Control/v1r67/modules:${ROOTSYS}/lib/root
ROOT_linkopts=`$(ROOT_BIN)/root-config --glibs` -lEG -lGeom -lMinuit -lFumili -lXMLIO
ROOT_cppflags=-DROOT_USED `$(ROOT_BIN)/root-config --cflags` -Wno-long-long
ROOT_stamps=$(ROOTROOT)/$(ROOTCONFIG)/ROOT.stamp
dict=../dict/
rootcint=$(ROOT_BIN)/rootcint
psycheCore_cppflags=-DPSYCHECORE_USED -fPIC
psycheCore_linkopts=-L$(PSYCHECOREROOT)/$(psycheCore_tag) -lpsycheCore
psycheCore_stamps= $(PSYCHECOREROOT)/$(psycheCore_tag)/psycheCore.stamp
highlandCore_cppflags=-DHIGHLANDCORE_USED -fPIC
highlandCore_linkopts=-L$(HIGHLANDCOREROOT)/$(highlandCore_tag) -lhighlandCore 
highlandCore_stamps= $(HIGHLANDCOREROOT)/$(highlandCore_tag)/highlandCore.stamp
highlandTools_cppflags=-DHIGHLANDTOOLS_USED -fPIC
highlandTools_linkopts=-L$(HIGHLANDTOOLSROOT)/$(highlandTools_tag) -lhighlandTools 
highlandTools_stamps= $(HIGHLANDTOOLSROOT)/$(highlandTools_tag)/highlandTools.stamp
register_dependencies= version  version  version  version 
tag=Linux-x86_64
package=highlandTools
version=v2r17p1
PACKAGE_ROOT=$(HIGHLANDTOOLSROOT)
srcdir=../src
bin=../$(highlandTools_tag)/
BIN=/mnt/data/backup/home/cfrancois/T2K2/highland2/highlandTools/v2r17p1/$(highlandTools_tag)/
javabin=../classes/
mgrdir=cmt
project=cfrancois
cmt_installarea_paths= $(cmt_installarea_prefix)/$(CMTCONFIG)/bin
use_linkopts= $(cmt_installarea_linkopts)   $(highlandTools_linkopts)  $(highlandCore_linkopts)  $(psycheCore_linkopts)  $(psychePolicy_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
T2K_installarea_prefix=$(cmt_installarea_prefix)
T2K_installarea_prefix_remove=$(T2K_installarea_prefix)
cfrancois_installarea_prefix=$(cmt_installarea_prefix)
cfrancois_installarea_prefix_remove=$(cfrancois_installarea_prefix)
use_requirements=requirements $(CMTROOT)/mgr/requirements $(HIGHLANDCOREROOT)/cmt/requirements $(PSYCHECOREROOT)/cmt/requirements $(PSYCHEPOLICYROOT)/cmt/requirements $(ROOTROOT)/cmt/requirements $(MYSQLROOT)/cmt/requirements $(GSLROOT)/cmt/requirements $(EXTERNROOT)/cmt/requirements 
use_includes= $(ppcmd)"$(highlandCore_root)/src" $(ppcmd)"$(psycheCore_root)/src" $(ppcmd)"$(psychePolicy_root)/src" $(ppcmd)"/mnt/data/T2K/frozen/ROOT/v5r34p09n04/Linux-x86_64/include/root" $(ppcmd)"none" $(ppcmd)"/mnt/data/T2K/frozen/MYSQL/v5r051an01/Linux-x86_64/include/mysql" $(ppcmd)"/mnt/data/T2K/frozen/GSL/v1r15p0n00/Linux-x86_64/include" $(ppcmd)"$(EXTERN_root)/src" 
use_fincludes= $(use_includes)
use_stamps= $(highlandTools_stamps)  $(highlandCore_stamps)  $(psycheCore_stamps)  $(psychePolicy_stamps)  $(ROOT_stamps)  $(MYSQL_stamps)  $(GSL_stamps)  $(EXTERN_stamps) 
use_cflags=  $(highlandTools_cflags)  $(highlandCore_cflags)  $(psycheCore_cflags)  $(psychePolicy_cflags)  $(ROOT_cflags)  $(MYSQL_cflags)  $(GSL_cflags)  $(EXTERN_cflags) 
use_pp_cflags=  $(highlandTools_pp_cflags)  $(highlandCore_pp_cflags)  $(psycheCore_pp_cflags)  $(psychePolicy_pp_cflags)  $(ROOT_pp_cflags)  $(MYSQL_pp_cflags)  $(GSL_pp_cflags)  $(EXTERN_pp_cflags) 
use_cppflags=  $(highlandTools_cppflags)  $(highlandCore_cppflags)  $(psycheCore_cppflags)  $(psychePolicy_cppflags)  $(ROOT_cppflags)  $(MYSQL_cppflags)  $(GSL_cppflags)  $(EXTERN_cppflags) 
use_pp_cppflags=  $(highlandTools_pp_cppflags)  $(highlandCore_pp_cppflags)  $(psycheCore_pp_cppflags)  $(psychePolicy_pp_cppflags)  $(ROOT_pp_cppflags)  $(MYSQL_pp_cppflags)  $(GSL_pp_cppflags)  $(EXTERN_pp_cppflags) 
use_fflags=  $(highlandTools_fflags)  $(highlandCore_fflags)  $(psycheCore_fflags)  $(psychePolicy_fflags)  $(ROOT_fflags)  $(MYSQL_fflags)  $(GSL_fflags)  $(EXTERN_fflags) 
use_pp_fflags=  $(highlandTools_pp_fflags)  $(highlandCore_pp_fflags)  $(psycheCore_pp_fflags)  $(psychePolicy_pp_fflags)  $(ROOT_pp_fflags)  $(MYSQL_pp_fflags)  $(GSL_pp_fflags)  $(EXTERN_pp_fflags) 
use_libraries= $(highlandCore_libraries)  $(psycheCore_libraries)  $(psychePolicy_libraries)  $(ROOT_libraries)  $(MYSQL_libraries)  $(GSL_libraries)  $(EXTERN_libraries) 
includes= $(ppcmd)"$(srcdir)" $(use_includes)
fincludes= $(includes)
highlandTools_use_linkopts=  $(highlandTools_linkopts)  $(highlandCore_linkopts)  $(psycheCore_linkopts)  $(psychePolicy_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
constituents= dictionary highlandTools 
all_constituents= $(constituents)
constituentsclean= highlandToolsclean dictionaryclean 
all_constituentsclean= $(constituentsclean)
cmt_actions_constituents= make 
cmt_actions_constituentsclean= makeclean 
documentation_constituents= doxygen 
documentation_constituentsclean= doxygenclean 
