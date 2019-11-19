cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project(nd280AnalysisTools)

if(NOT DEFINED IS_ND_CONFIGURING)
  message(FATAL_ERROR "CMake manually invoked, you should configure from the main nd280 Package.")
elseif(NOT ${IS_ND_CONFIGURING})
  include (/home/luke/projects/t2k/dev/nd280/.highland2work/v11r35/nd280Analysis/v1r50/cmake/utils/Use_ND_CMake_Utils.cmake)
else()
  include (${ND_CMAKE_UTILS_DIR}/Use_ND_CMake_Utils.cmake)
endif()

#Package set up
#################################

set(nd280AnalysisTools_VERSION_MAJOR 1)
set(nd280AnalysisTools_VERSION_MINOR 10)
set(nd280AnalysisTools_VERSION_REVISION 0)

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
  set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
elseif(CMAKE_INSTALL_PREFIX STREQUAL "")
  set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
elseif(DEFINED ENV{CMT_INSTALL_PREFIX})
  set(CMAKE_INSTALL_PREFIX $ENV{CMT_INSTALL_PREFIX})
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
endif()

set(CMAKE_SKIP_RPATH True)
set(CMAKE_BUILD_TYPE DEBUG)

cmessage(STATUS "[INSTALL]: Installing into: ${CMAKE_INSTALL_PREFIX}")

#Compiler flags
#################################

if(NOT ${IS_ND_CONFIGURING})

  if(NOT DEFINED CMTCPP
    OR NOT DEFINED CMTCPPFLAGS
    OR NOT DEFINED CMTCPPDEBUGFLAGS
    OR NOT DEFINED CMTCPPPPFLAGS
    OR NOT DEFINED CMTINCLUDES
    OR NOT DEFINED CMTLINKFLAGS
    OR NOT DEFINED CMTLINKOPTS)
    cmessage(FATAL_ERROR "CMT Options not passed in. Are you trying to invoke this CMakeLists.txt manually?")
  endif()

  set(CMAKE_CXX_COMPILER ${CMTCPP})
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMTCPPFLAGS} ${CMTCPPDEBUGFLAGS} ${CMTCPPPPFLAGS} ${CMTINCLUDES} ${ROOT_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMTCPPFLAGS} ${CMTCPPPPFLAGS} ${CMTINCLUDES} ${ROOT_CXX_FLAGS}")

  if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_DEBUG} ${CMTLINKFLAGS} ${CMTLINKOPTS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
  elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${CMTLINKFLAGS} ${CMTLINKOPTS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  else()
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${CMTLINKFLAGS} ${CMTLINKOPTS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  endif()

  cmessage (STATUS "C++ Compiler      : " ${CXX_COMPILER_NAME})
  cmessage (STATUS "    Release flags : " ${CMAKE_CXX_FLAGS})
  cmessage (STATUS "    Link Flags    : " ${CMAKE_LINK_FLAGS})

else()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ROOT_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ROOT_CXX_FLAGS}")

  if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_DEBUG} ${ROOT_LD_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
  elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${ROOT_LD_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  else()
    set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS_RELEASE} ${ROOT_LD_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  endif()

  cmessage (STATUS "C++ Compiler      : " ${CXX_COMPILER_NAME})
  cmessage (STATUS "    CXX  Flags    : " ${CMAKE_CXX_FLAGS})
  cmessage (STATUS "    Link Flags    : " ${CMAKE_LINK_FLAGS})
endif()


string(REPLACE "lib" ""
  PROJECT_LIBNAME ${ROOT_PROJECT_NAME})
cmessage(STATUS "ROOT file used to generate project: ${ROOT_PROJECT_FILE}")

#################################

set(nd280AnalysisTools_SUBDIRS
  src
  AnalysisTools
  app)

#add directories for finding files
foreach (dir ${nd280AnalysisTools_SUBDIRS})
  if(EXISTS ${PROJECT_SOURCE_DIR}/${dir})
    include_directories(${dir})
  else()
    cmessage(FATAL_ERROR "Directory ${PROJECT_SOURCE_DIR}/${dir} doesn't exist and it was expected to.")
  endif()
endforeach()

include_directories(${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME})

foreach (dir ${nd280AnalysisTools_SUBDIRS})
  add_subdirectory(${dir})
endforeach()
