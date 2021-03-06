# 
# alps-hdf5 cmake configuration script.
# 

cmake_minimum_required (VERSION 2.8.12)

# define project name
project (alps-hdf5 CXX)
message ("\nALPSCore module : ${PROJECT_NAME}\n")

# add common modules from ../common/cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../common/cmake)

include(ALPSCommonModuleDefinitions)

gen_documentation()
if (DocumentationOnly)
  return()
endif()

add_this_package(archive)

include(ALPSEnableMPI)

add_boost(filesystem system program_options)
add_hdf5()

add_alps_package(alps-utilities)

add_testing()
gen_pkg_config()
gen_cfg_module()
