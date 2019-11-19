#!/usr/bin/env sh

if [ "${1}" == "clean" ]; then
  CLEAN=1
fi

if [ "${1}" == "compile" ]; then
  COMPILE=1
fi

if [ "${CLEAN}" ]; then

  CLEANROOT="${2}"
  CLEANCONFIG="${3}"
  if [ ! "${CLEANROOT}" ] || [ ! "${CLEANCONFIG}" ]; then
    echo -e -n "Either \n\tND280ANALYSISTOOLSROOT: ${CLEANROOT} "
    echo -e -n "or \n\tND280ANALYSISTOOLSCONFIG: ${CLEANCONFIG} "
    echo -e "was not defined, did you source the setup.sh?"
    exit 1
  fi

  #Seems stupid but CMT wants to touch a file in there...
  if [ ! -e "${CLEANROOT}/${CLEANCONFIG}" ]; then
    mkdir "${CLEANROOT}/${CLEANCONFIG}"
  else
    echo "Cleaning ${CLEANROOT}/${CLEANCONFIG} ..."
    rm -r ${CLEANROOT}/${CLEANCONFIG}/*
  fi
  #So stupid but CMT expects this to be there.
  touch ${CLEANROOT}/${CLEANCONFIG}/ND280ANALYSISTOOLS.stamp
fi
if [ "${COMPILE}" ]; then

  COMPILEROOT="${2}"
  COMPILECONFIG="${3}"

  if [ ! "${COMPILEROOT}" ] || [ ! "${COMPILECONFIG}" ]; then
    echo -e -n "Either \n\tND280ANALYSISTOOLSROOT: ${CLEANROOT} "
    echo -e -n "or \n\tND280ANALYSISTOOLSCONFIG: ${CLEANCONFIG} "
    echo -e "was not defined, did you source the setup.sh?"
    exit 1
  fi

  CMTCPP=$(cmt show macro_value cpp)
  BUILDER_CPPFLAGS=$(cmt show macro_value cppflags | sed "s/\s\+/ /g")
  BUILDER_USECPPFLAGS=$(cmt show macro_value use_cppflags | sed "s/\s\+/ /g")
  CMTCPPFLAGS="${BUILDER_CPPFLAGS} ${BUILDER_USECPPFLAGS} $(cmt show macro_value cpp11_cppflags | sed 's/\s\+/ /g')"
  CMTCPPDEBUGFLAGS=$(cmt show macro_value cppdebugflags | sed "s/\s\+/ /g")
  CMTCPPPPFLAGS=$(cmt show macro_value pp_cppflags | sed "s/\s\+/ /g")
  #Fixes CMT being special -INone, -I../src, instead of -I${ND280ANALYSISTOOLSROOT}/src
  CMTINCLUDES=$(cmt show macro_value includes | sed "s:\.\./src:${COMPILEROOT}/src:g" | sed "s/\"//g" | sed "s/-Inone//g" | sed "s/\s\+/ /g")
  CMTLINKFLAGS=$(cmt show macro_value cpplinkflags | sed "s/\s\+/ /g")
  CMTLINKOPTS=$(cmt show macro_value use_linkopts | sed "s/\s\+/ /g")

  if [ ! -e "${COMPILEROOT}/${COMPILECONFIG}" ]; then
    echo "Making ${COMPILEROOT}/${COMPILECONFIG}/"
    mkdir ${COMPILEROOT}/${COMPILECONFIG}
  fi

  cd ${COMPILEROOT}/${COMPILECONFIG}

  if [ ! -e "${COMPILEROOT}/${COMPILECONFIG}/Makefile" ]; then
    mkdir -p ${COMPILEROOT}/${COMPILECONFIG}/in/

    cat ${OAEVENTROOT}/src/Tpackage_version.hxx.in | \
      sed "s/%PACKAGE%/@PROJECT_NAME@/g" > \
      ${COMPILEROOT}/${COMPILECONFIG}/in/Tpackage_version.hxx.in
    cat ${OAEVENTROOT}/src/Tpackage_version.cxx.in | \
      sed "s/%PACKAGE%/@PROJECT_NAME@/g" > \
      ${COMPILEROOT}/${COMPILECONFIG}/in/Tpackage_version.cxx.in
    cat ${OAEVENTROOT}/src/Tpackage_version_LinkDef.in | \
      sed "s/%PACKAGE%/@PROJECT_NAME@/g" > \
      ${COMPILEROOT}/${COMPILECONFIG}/in/Tpackage_version_LinkDef.h.in

    echo "Invoking CMake to configure build in ${COMPILEROOT}/${COMPILECONFIG}"
    cmake ${COMPILEROOT} \
      -DCMTCPP="${CMTCPP}" \
      -DCMTCPPFLAGS="${CMTCPPFLAGS}" \
      -DCMTCPPDEBUGFLAGS="${CMTCPPDEBUGFLAGS}" \
      -DCMTCPPPPFLAGS="${CMTCPPPPFLAGS}" \
      -DCMTINCLUDES="${CMTINCLUDES}" \
      -DCMTLINKFLAGS="${CMTLINKFLAGS}" \
      -DCMTLINKOPTS="${CMTLINKOPTS}" \
      -DROOT_PROJECT_NAME="libReadoaAnalysis" \
      -DROOT_PROJECT_FILE=${COMPILEROOT}/src/ToCompile.root \
      -DIS_ND_CONFIGURING=off \
      -DCMAKE_BUILD_TYPE=DEBUG

  fi

  echo "Building... -j $(cmt show macro_value ND280ANALYSISTOOLS_NCPUCOMPILE)"
  make install -j $(cmt show macro_value ND280ANALYSISTOOLS_NCPUCOMPILE)
fi
