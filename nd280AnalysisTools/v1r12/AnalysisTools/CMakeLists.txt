set(TOOLS_TO_COMPILE
  TMatrixWeighting.cxx
  TSpectrumWeighting.cxx
  TVNeutrinoGeneratorAnalysisUtils.cxx
  TGENIEAnalysisUtils.cxx
  TNEUTAnalysisUtils.cxx
  TGRooTrackerUtils.cxx
  TTruthUtils.cxx
  Stage2GasSelection.cxx
  TPCPOTCalculator.cxx
  P0DNuMuUtils.cxx
  TNUANCEAnalysisUtils.cxx
  TECALPidDefs.cxx
  TECALPidLikelihoodHypothesisTest.cxx
  TECALPidLikelihood.cxx
  TECALEMEnergyCorrection.cxx
  TTrackerECALReconModuleTools.cxx
  TTrackerAnalysisUtils.cxx
  TCheatingAnalysisUtils.cxx
  TMiscAnalysisUtils.cxx)

foreach(tool ${TOOLS_TO_COMPILE})
  get_filename_component(toolname ${tool} NAME_WE)
  add_library(${toolname} SHARED ${tool})
  add_dependencies(${toolname} ${PROJECT_LIBNAME})
  target_include_directories(${toolname} PUBLIC ${CMAKE_BINARY_DIR})
  install(TARGETS ${toolname} DESTINATION lib)
  install(FILES ${toolname}.hxx DESTINATION include)
  #Down stream packages expect them to live here (use #include "AnalysisTools/XYZ.hxx")
  install(FILES ${toolname}.hxx DESTINATION AnalysisTools)
endforeach()
