set(FILESTODICT
  TND280TrackList.hxx
  TND280TrackPropagator.hxx
  TND280Shower.hxx
  TND280Track.hxx
  ToaAnalysisEventDisplayUtils.hxx
  TEventDisplay.hxx
  TND280MagField.hxx
  )

set(IMPLFILES
  TControlTab.cxx
  TDisplayedTrackerModule.cxx
  TND280TrackList.cxx
  TDisplayedAnalysisModuleBase.cxx
  TDisplayedTruthModule.cxx
  TND280TrackPropagator.cxx
  TDisplayedGlobalModule.cxx
  ToaAnalysisEventDisplayUtils.cxx
  TDisplayedP0DECALModule.cxx
  TND280MagField.cxx
  TOstreamOverloads.cxx
  TDisplayedP0DModule.cxx
  TND280Shower.cxx
  TSimpleGeometry.cxx
  TDisplayedTrackerECalModule.cxx
  TND280Track.cxx
  TEventDisplay.cxx)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

foreach(ftd ${FILESTODICT})
  get_filename_component(FTDName ${ftd} NAME_WE)
  set(OutputDict ${CMAKE_CURRENT_BINARY_DIR}/${FTDName}Dict)
  GenROOTDictionary(
    ${OutputDict}
    ${ftd}
    ${FTDName}_LinkDef.h
    )
  set(IMPLFILES ${IMPLFILES};${OutputDict}.cxx)
endforeach()

add_library(nd280AnaEveDisplay SHARED ${IMPLFILES})

set_target_properties(nd280AnaEveDisplay PROPERTIES COMPILE_FLAGS " -DTTRACKERECALRECONMODULE_USE_EXP ")

target_link_libraries(nd280AnaEveDisplay ${PROJECT_LIBNAME})

install(TARGETS nd280AnaEveDisplay DESTINATION lib)


