add_executable(Stage2GasSelection.exe Stage2GasSelection.cxx)

target_include_directories(Stage2GasSelection.exe PUBLIC
  ${PROJECT_SOURCE_DIR}/AnalysisTools
  ${PROJECT_BINARY_DIR}/libReadoaAnalysis)
target_link_libraries(Stage2GasSelection.exe
  Stage2GasSelection
  TTruthUtils
  ReadoaAnalysis)
set_target_properties(Stage2GasSelection.exe PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})

add_executable(TPCPOTCalculator.exe TPCPOTCalculator.cxx)

target_include_directories(TPCPOTCalculator.exe PUBLIC
  ${PROJECT_SOURCE_DIR}/AnalysisTools
  ${PROJECT_BINARY_DIR}/libReadoaAnalysis)
target_link_libraries(TPCPOTCalculator.exe
  TPCPOTCalculator
  ReadoaAnalysis)
set_target_properties(TPCPOTCalculator.exe PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})

add_executable(AnaToolsEventDisplay.exe EventDisplay.cxx)

target_include_directories(AnaToolsEventDisplay.exe PUBLIC
  ${PROJECT_SOURCE_DIR}/src/EventDisplay
  ${PROJECT_BINARY_DIR}/libReadoaAnalysis)
target_link_libraries(AnaToolsEventDisplay.exe
  nd280AnaEveDisplay
  ReadoaAnalysis
  TNEUTAnalysisUtils
  TGENIEAnalysisUtils
  TTruthUtils
  TTrackerECALReconModuleTools
  -lEve)
set_target_properties(AnaToolsEventDisplay.exe PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})


install (TARGETS Stage2GasSelection.exe DESTINATION bin)
install (TARGETS TPCPOTCalculator.exe DESTINATION bin)
install (TARGETS AnaToolsEventDisplay.exe DESTINATION bin)
