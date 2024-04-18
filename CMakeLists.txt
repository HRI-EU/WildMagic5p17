cmake_minimum_required(VERSION 3.16)
PROJECT(WM5)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

################################################################################
#
# Setup exported target collection
#
################################################################################

# Setup WM5_EXPORT_LIBRARIES so that CMakeLists.txt can modify it
SET(WM5_EXPORT_LIBRARIES "" CACHE INTERNAL "")

# Create a macro for adding an export library
FUNCTION(WM5_ADD_EXPORT_LIBRARY library)
  SET(WM5_EXPORT_LIBRARIES ${WM5_EXPORT_LIBRARIES} ${library} CACHE INTERNAL "")
ENDFUNCTION()

###############################################################################
#
# Wm5Core library
#
###############################################################################
FILE(GLOB_RECURSE WM5CORE_SRC CONFIGURE_DEPENDS LibCore/*.cpp)

ADD_LIBRARY(Wm5Core SHARED ${WM5CORE_SRC})

IF(UNIX)
  SET_TARGET_PROPERTIES(Wm5Core PROPERTIES COMPILE_FLAGS "-O2 -DNDEBUG -c -D__LINUX__ -fPIC")
  #TARGET_COMPILE_DEFINITIONS(Wm5Core PRIVATE -O2 -DNDEBUG -c -D__LINUX__ -fPIC)
  TARGET_COMPILE_DEFINITIONS(Wm5Core INTERFACE -DUSE_WM5)
ELSE()
  TARGET_COMPILE_DEFINITIONS(Wm5Core PRIVATE -DWM5_CORE_DLL_EXPORT)
  TARGET_COMPILE_DEFINITIONS(Wm5Core INTERFACE -DWM5_CORE_DLL_IMPORT -DUSE_WM5)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(Wm5Core SYSTEM PUBLIC
    # Header source locations of LibCore
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Assert>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/DataTypes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/InputOutput>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Memory>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/ObjectSystems>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Threading>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Time>
   
    # Header install location
    $<INSTALL_INTERFACE:include/WM5>)

  # copy the .dll file to the same folder as the executable
  IF (WIN32)
    ADD_CUSTOM_COMMAND(
      TARGET Wm5Core POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Wm5Core.dll
      ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Wm5Core.dll)
  ENDIF()
  
###############################################################################
#
# Wm5Mathematics library
#
###############################################################################
FILE(GLOB_RECURSE WM5MATH_SRC CONFIGURE_DEPENDS LibMathematics/*.cpp)

ADD_LIBRARY(Wm5Mathematics SHARED ${WM5MATH_SRC})
TARGET_LINK_LIBRARIES(Wm5Mathematics Wm5Core)

IF(UNIX)
  SET_TARGET_PROPERTIES(Wm5Mathematics PROPERTIES COMPILE_FLAGS "-O2 -DNDEBUG -c -D__LINUX__ -fPIC")
  #TARGET_COMPILE_DEFINITIONS(Wm5Mathematics PRIVATE -O2 -DNDEBUG -c -D__LINUX__ -fPIC)
ELSE()
  TARGET_COMPILE_DEFINITIONS(Wm5Mathematics PRIVATE -DWM5_MATHEMATICS_DLL_EXPORT)
  TARGET_COMPILE_DEFINITIONS(Wm5Mathematics INTERFACE -DWM5_MATHEMATICS_DLL_IMPORT)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(Wm5Mathematics SYSTEM PUBLIC 
    # Header source locations of LibCore
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Assert>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/DataTypes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/InputOutput>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Memory>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/ObjectSystems>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Threading>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibCore/Time>
   
    # Header source locations of LibMathematics
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Algebra>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Approximation>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Base>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/ComputationalGeometry>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Containment>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/CurvesSurfacesVolumes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Distance>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Interpolation>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Intersection>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Meshes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Miscellaneous>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/NumericalAnalysis>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Objects2D>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Objects3D>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Query>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/LibMathematics/Rational>
   
    # Header install location
    $<INSTALL_INTERFACE:include/WM5>)

  # copy the .dll file to the same folder as the executable
  IF (WIN32)
    ADD_CUSTOM_COMMAND(
      TARGET Wm5Mathematics POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Wm5Mathematics.dll
      ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/Wm5Mathematics.dll)
  ENDIF()
 
###############################################################################
# Install the libraries
###############################################################################
INSTALL(TARGETS Wm5Core Wm5Mathematics EXPORT Wm5Export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/Wm5)

###############################################################################
# Mark for export
###############################################################################
WM5_ADD_EXPORT_LIBRARY(Wm5Core)
WM5_ADD_EXPORT_LIBRARY(Wm5Mathematics)

################################################################################
# Target export script
################################################################################
INCLUDE(ExportWm5)
