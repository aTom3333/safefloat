cmake_minimum_required(VERSION 3.6.1)
project(safefloat)
# The version number.
set (safefloat_VERSION_MAJOR 0)
set (safefloat_VERSION_MINOR 0)
set (safefloat_VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("debug mode")
    include(CodeCoverage)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0 -fprofile-arcs -ftest-coverage")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)


add_library(safefloat INTERFACE)

#Boost
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(include ${Boost_INCLUDE_DIRS})

# Check for standard to use
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
if(HAVE_FLAG_STD_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=c++1z")
endif()

enable_testing()
# Unit tests
FILE(GLOB TestSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*_test.cpp)
foreach(testSrc ${TestSources})
        get_filename_component(testName ${testSrc} NAME_WE)
        add_executable(${testName} test/main-test.cpp ${testSrc})
        target_link_libraries(${testName} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	add_test(${testName} ${testName})
endforeach(testSrc)

# Tests that should compile 
FILE(GLOB TestCompileSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test-compile/compiles/*_test.cpp)
foreach(testCompSrc ${TestCompileSources})
        get_filename_component(testCompName ${testCompSrc} NAME_WE)
        add_executable(${testCompName} ${testCompSrc})
        set_target_properties(${testCompName} PROPERTIES
                              EXCLUDE_FROM_ALL TRUE
                              EXCLUDE_FROM_DEFAULT_BUILD TRUE)
        target_include_directories(${testCompName} PUBLIC test-compile/compiles)
        add_test(NAME ${testCompName}
                 COMMAND ${CMAKE_COMMAND} --build . --target ${testCompName}
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach(testCompSrc)

# Tests that should fail compilation
FILE(GLOB TestCompileFailSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test-compile/compile-fails/*_test.cpp)
foreach(testCmpFailSrc ${TestCompileFailSources})
        get_filename_component(testCmpFailName ${testCmpFailSrc} NAME_WE)
        add_executable(${testCmpFailName} ${testCmpFailSrc})
        set_target_properties(${testCmpFailName} PROPERTIES
                              EXCLUDE_FROM_ALL TRUE
                              EXCLUDE_FROM_DEFAULT_BUILD TRUE)
        add_test(NAME ${testCmpFailName}
                 COMMAND ${CMAKE_COMMAND} --build . --target ${testCmpFailName}
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        set_tests_properties(${testCmpFailName} PROPERTIES WILL_FAIL TRUE)
endforeach(testCmpFailSrc)

# Examples
FILE(GLOB ExampleSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/example_*.cpp)
foreach(exampleSrc ${ExampleSources})
        get_filename_component(exampleName ${exampleSrc} NAME_WE)
        add_executable(${exampleName} ${exampleSrc})
endforeach(exampleSrc)

#Library Headers
add_executable(safefloat_headers include)
set_target_properties(safefloat_headers PROPERTIES
                                                 EXCLUDE_FROM_ALL TRUE
                                                 EXCLUDE_FROM_DEFAULT_BUILD TRUE
                                                 LINKER_LANGUAGE CXX)



