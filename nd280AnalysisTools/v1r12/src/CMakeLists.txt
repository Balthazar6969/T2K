#Build helper exectuable for dumping out class sources.
add_executable(DumpNDClasses DumpNDClasses.cxx)
set_target_properties(DumpNDClasses PROPERTIES LINK_FLAGS ${CMAKE_LINK_FLAGS})

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectSource.cxx"
  "${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}LinkDef.h"
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/DumpNDClasses
  ARGS ${ROOT_PROJECT_FILE}
    ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}
    "ND::"
  VERBATIM)

add_custom_target(${ROOT_PROJECT_NAME}_sources
  DEPENDS
  "${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectSource.cxx")

add_dependencies(${ROOT_PROJECT_NAME}_sources DumpNDClasses)

GenROOTDictionary(
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectDict
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectHeaders.h
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}LinkDef.h
  )

add_custom_target(${ROOT_PROJECT_NAME}ProjectDict
  DEPENDS
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectDict.cxx
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectDict.h )
add_dependencies(${ROOT_PROJECT_NAME}ProjectDict ${ROOT_PROJECT_NAME}_sources)


#ProjectSource.cxx includes ProjectDict.cxx, so no need to add to compilation.
set(ROAA_SOURCEFILES
  ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectSource.cxx)

add_library(${PROJECT_LIBNAME} SHARED ${ROAA_SOURCEFILES})
add_dependencies(${PROJECT_LIBNAME} ${ROOT_PROJECT_NAME}ProjectDict)

set_target_properties(${PROJECT_LIBNAME} PROPERTIES VERSION
  "${nd280AnalysisTools_VERSION_MAJOR}.${nd280AnalysisTools_VERSION_MINOR}.${nd280AnalysisTools_VERSION_REVISION}")

#Install in both places because downstream packages are rubbish.
install (TARGETS ${PROJECT_LIBNAME} DESTINATION AnalysisTools/${ROOT_PROJECT_NAME})
install (TARGETS ${PROJECT_LIBNAME} DESTINATION lib)

install (FILES ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectHeaders.h
  DESTINATION include)
install (FILES ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectHeaders.h
  DESTINATION AnalysisTools/${ROOT_PROJECT_NAME})
install (FILES ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectInstances.h
  DESTINATION include)
install (FILES ${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/${ROOT_PROJECT_NAME}ProjectInstances.h
  DESTINATION AnalysisTools/${ROOT_PROJECT_NAME})
install(
  CODE "FILE(GLOB ROAA_HEADERS \"${CMAKE_BINARY_DIR}/${ROOT_PROJECT_NAME}/ND__*.h\" )"
  CODE "FILE(INSTALL \${ROAA_HEADERS} DESTINATION AnalysisTools/${ROOT_PROJECT_NAME})"
  CODE "FILE(INSTALL \${ROAA_HEADERS} DESTINATION include)"
)

add_subdirectory(EventDisplay)
