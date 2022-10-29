#--------------------------------------------------------------
# CMake file for HydroChrono library and its demos
# Eventually will add SWIG and python lines to generate python
# library from C++ code
#--------------------------------------------------------------

cmake_minimum_required(VERSION 3.18.2)

#--------------------------------------------------------------
# Modify the project name if you want: 
#--------------------------------------------------------------

project(HydroChrono)
# project(HydroChrono CXX)

# find required packages and libraries to make HydroChrono library
# set (LIB_TYPE STATIC) # or SHARED
# string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)

find_package(CHRONO COMPONENTS Irrlicht CONFIG
  PATHS "/Users/rhys/Code/chrono/chrono_ws/install/lib/cmake"
)
set(Chrono_INCLUDE_DIRS "/Users/rhys/Code/chrono/chrono_ws/install/include")
set(CHRONO_LIBRARY_DIRS "/Users/rhys/Code/chrono/chrono_ws/install/lib")
set(CHRONO_LIBRARIES
  ${CHRONO_LIBRARY_DIRS}/libChronoEngine_irrlicht.dylib
  ${CHRONO_LIBRARY_DIRS}/libChronoEngine_postprocess.dylib
  ${CHRONO_LIBRARY_DIRS}/libChronoEngine.dylib
  ${CHRONO_LIBRARY_DIRS}/libChronoModels_robot.dylib
)

message("CHRONO_FOUND: ${Chrono_FOUND}")
message("CHRONO_INCLUDE_DIRS: ${CHRONO_INCLUDE_DIRS}")
message("CHRONO_LIBRARY_DIRS: ${CHRONO_LIBRARY_DIRS}")
message("CHRONO_LIBRARIES: ${CHRONO_LIBRARIES}")

# find_package(HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
# message("HDF5_FOUND: ${HDF5_FOUND}")
set(HDF5_INCLUDE_DIRS "/usr/local/opt/hdf5/include")
set(HDF5_LIBRARY_DIRS "/usr/local/opt/hdf5/lib")
set(HDF5_LIBRARIES hdf5 hdf5_cpp)
message("HDF5_INCLUDE_DIRS: ${HDF5_INCLUDE_DIRS}")
message("HDF5_LIBRARY_DIRS: ${HDF5_LIBRARY_DIRS}")
message("HDF5_LIBRARIES: ${HDF5_LIBRARIES}")

find_package(EIGEN3 REQUIRED)

# find_package(IRRLICHT REQUIRED)
# message("IRRLICHT_FOUND: ${IRRLICHT_FOUND}")
set(IRRLICHT_INCLUDE_DIRS "/usr/local/opt/irrlicht/include/irrlicht")
set(IRRLICHT_LIBRARY_DIRS "/usr/local/opt/irrlicht/lib")
message("IRRLICHT_INCLUDE_DIRS: ${IRRLICHT_INCLUDE_DIRS}")
message("IRRLICHT_LIBRARY_DIRS: ${IRRLICHT_LIBRARY_DIRS}")


#find_package(SWIG REQUIRED)
#find_package(PythonLibs)
#include(${SWIG_USE_FILE})

include_directories(
  ${HDF5_INCLUDE_DIRS}
  ${CHRONO_INCLUDE_DIRS}
  ${IRRLICHT_INCLUDE_DIRS}
)

link_directories(
  ${HDF5_LIBRARY_DIRS}
  ${CHRONO_LIBRARY_DIRS}
  ${IRRLICHT_LIBRARY_DIRS}
)

#include_directories(${HDF5_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR} ${CHRONO_INCLUDE_DIRS})

# set(PROJECT_LIBS ${HDF5_LIBS} ${HDF5_LIBRARIES})
# set(LINK_LIBS ${LINK_LIBS}
#   ${CHRONO_LIBRARIES}
#   ${HDF5_CXX_${LIB_TYPE}_LIBRARY}
# )
#SET(CMAKE_SWIG_FLAGS "")

#-----------------------------------------------------------------------------
# Fix for VS 2017 15.8 and newer to handle alignment specification with Eigen
#-----------------------------------------------------------------------------

# if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#   if(MSVC AND ${MSVC_VERSION} GREATER_EQUAL 1915)
#     add_definitions( "-D_ENABLE_EXTENDED_ALIGNED_STORAGE" )
#   endif()
# endif()

#--------------------------------------------------------------
# Tweaks to disable some warnings with MSVC
#--------------------------------------------------------------
# if(MSVC)
#     add_definitions("-D_CRT_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
#     add_definitions("-D_SCL_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
#     add_definitions( "-DNOMINMAX" )                # do not use MSVC's min/max macros
#     add_compile_options(/wd4275)                   # disable warnings triggered by Irrlicht
#     add_compile_options(/wd4251)                   # class needs to have dll-interface
# else()
#     set(EXTRA_COMPILE_FLAGS "")
# endif()

#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++ 
# files in your project. 
#--------------------------------------------------------------

add_library(HydroChrono SHARED
  src/hydro_forces.cpp
  src/hydro_forces.h
)
target_link_libraries(HydroChrono
  ${CHRONO_LIBRARIES}
  Eigen3::Eigen
  ${HDF5_LIBRARIES}
)

add_executable(sphere_decay
  demos/sphere/sphere_decay.cpp
)
target_link_libraries(sphere_decay
  HydroChrono
  ${CHRONO_LIBRARIES}
  Eigen3::Eigen
  ${HDF5_LIBRARIES}
)
target_include_directories(sphere_decay
  PUBLIC
    src
)

add_executable(sphere_reg_waves
  demos/sphere/sphere_reg_waves.cpp
)
target_link_libraries(sphere_reg_waves
  HydroChrono
  ${CHRONO_LIBRARIES}
  Eigen3::Eigen
  ${HDF5_LIBRARIES}
)
target_include_directories(sphere_reg_waves
  PUBLIC
    src
)

add_executable(rm3_decay
  demos/rm3/rm3_decay.cpp
)
target_link_libraries(rm3_decay
  HydroChrono
  ${CHRONO_LIBRARIES}
  Eigen3::Eigen
  ${HDF5_LIBRARIES}
)
target_include_directories(rm3_decay
  PUBLIC
    src
)

add_executable(rm3_reg_waves
  demos/rm3/rm3_reg_waves.cpp
)
target_link_libraries(rm3_reg_waves
  HydroChrono
  ${CHRONO_LIBRARIES}
  Eigen3::Eigen
  ${HDF5_LIBRARIES}
)
target_include_directories(rm3_reg_waves
  PUBLIC
    src
)

# install
install(
  TARGETS
    HydroChrono
    sphere_decay
    sphere_reg_waves
    rm3_decay
    rm3_reg_waves
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

#SWIG_ADD_LIBRARY(py_wec_chrono LANGUAGE python SOURCES hydro_forces_no_viz.i)

# target_compile_features(HydroChrono PUBLIC cxx_std_17)

#--------------------------------------------------------------
# Set properties for your executable target
# 
# Note that here we define a macro CHRONO_DATA_DIR which will
# contain the path to the Chrono data directory, either in its
# source tree (if using a build version of Chrono), or in its
# install tree (if using an installed version of Chrono).
#--------------------------------------------------------------

# set_target_properties(HydroChrono PROPERTIES 
# 	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
# 	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
# 	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

# set_target_properties(sphere_decay PROPERTIES 
# 	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
# 	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
# 	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

# set_target_properties(sphere_reg_waves PROPERTIES 
# 	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
# 	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
# 	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

# set_target_properties(rm3_decay PROPERTIES 
# 	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
# 	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
# 	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

# set_target_properties(rm3_reg_waves PROPERTIES 
# 	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
# 	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
# 	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

#--------------------------------------------------------------
# Link to Chrono libraries and dependency libraries
#--------------------------------------------------------------

# target_link_libraries(HydroChrono ${LINK_LIBS})
# target_link_libraries(sphere_decay HydroChrono)
# target_link_libraries(sphere_reg_waves HydroChrono)
# target_link_libraries(rm3_decay HydroChrono)
# target_link_libraries(rm3_reg_waves HydroChrono)
#SWIG_LINK_LIBRARIES(py_wec_chrono wec_chrono ${PYTHON_LIBRARIES})

#--------------------------------------------------------------
# === 4 (OPTIONAL) ===
# 
# Optionally, add a custom command for copying all Chrono and
# dependency DLLs to the appropriate binary output folder.
# This function has effect only on Windows.
# 
# Note that you must first set EXECUTABLE_OUTPUT_PATH
# (this can simply be ${CMAKE_BINARY_DIR}, like in this example)
#--------------------------------------------------------------

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# add_DLL_copy_command("${CHRONO_DLLS}")
