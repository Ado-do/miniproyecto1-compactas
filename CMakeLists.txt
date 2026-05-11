cmake_minimum_required(VERSION 3.15)
project(miniproyecto1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Desactivar tests de libs
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE) # Specific to bench-lib

include(FetchContent) # Usar FetchContent en vez de git submodules :P
FetchContent_Declare(
    sdsl_lite
    GIT_REPOSITORY https://github.com/xxsds/sdsl-lite.git
    GIT_TAG        master
)
FetchContent_Declare(
    bench-lib
    GIT_REPOSITORY https://github.com/bletelier/bench-lib.git
    GIT_TAG        c70018f7b90a907f28e8aaade5082f1e7fbeefb1 # 30/03/26
)
FetchContent_MakeAvailable(sdsl_lite bench-lib) # Descarga y configura libs

add_library(miniproyecto1 STATIC
  src/bwt.cpp
  src/OccMyWT.cpp
)

target_include_directories(miniproyecto1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(miniproyecto1 PUBLIC sdsl-lite)

set(BUILD_TESTING ON CACHE BOOL "" FORCE)
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

target_compile_options(miniproyecto1 PRIVATE -O3 -Wall)
