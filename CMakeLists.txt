cmake_minimum_required(VERSION 3.10)

# set the project name
project(PCM_Examples VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set flags on need
set(CFLAGS)
set(PHI_FLAGS -mavx512f -mavx512cd -mavx512er -mavx512pf)
set(GPU_BOEHM_FLAGS -march=cascadelake)
set(HAEC_BOEHM_FLAGS -march=skylake-avx512)

#set pcm location
set(PCM_LOCATION ./thirdParty/pcm)
set(PCM -lpcm -L${CMAKE_CURRENT_LIST_DIR}/${PCM_LOCATION}/build/lib)
# pass the in formation about the shared library location to the linker
link_directories(${CMAKE_CURRENT_LIST_DIR}/${PCM_LOCATION}/build/lib)


# pass flags to compiler
add_definitions(${CFLAGS})
#add_definitions(${PHI_FLAGS})
#add_definitions(${GPU_BOEHM_FLAGS})
add_definitions(${HAEC_BOEHM_FLAGS})
#pass the information about the shared library location to the compiler
add_definitions(${PCM})


# build directory
set(CMAKE_BINARY_DIR "bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# include directories
include_directories(src/utils)
include_directories(src/algorithm)
include_directories(thirdParty/pcm/src)


# specify build targets / executables to build
add_executable(PCMExample src/benchmark/IsEvenBench.cpp)
#					 target name|file name of the shared library in any ofthe link directories
target_link_libraries(PCMExample libpcm.so)


