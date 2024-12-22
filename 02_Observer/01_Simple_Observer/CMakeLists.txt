cmake_minimum_required(VERSION 3.10)
project(observer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wno-error=deprecated-declarations")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -gdwarf-2 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# set(CUSTOM_NAME "test_classes")
# if(NOT DEFINED PROJECT_NAME)
#   set(PROJECT_NAME ${CUSTOM_NAME})
#   endif()
# project(${PROJECT_NAME})
file(GLOB SOURCES "*.cpp" "*.h")

add_executable(${PROJECT_NAME} ${SOURCES}) 
