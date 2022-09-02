cmake_minimum_required(VERSION 3.6.0)
project(appenv)

option(BUILD_DEBUG "Build a debug release" OFF)
option(BUILD_SHARED "Build a shared library" ON)
option(BUILD_STATIC "Build a static library" OFF)

#library version
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
	set(BUILD_VERSION "\"-debug\"")

else()
	set(CMAKE_BUILD_TYPE Release)
	set(BUILD_VERSION "\"\"")
endif()

add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
add_definitions(-DBUILD_VERSION=${BUILD_VERSION})

message("will build version ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}${BUILD_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directory.
include_directories("include")
set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

#Library building and installation.

if(${BUILD_SHARED})
	add_library(appenv_shared SHARED ${SOURCE})
	set_target_properties(appenv_shared PROPERTIES OUTPUT_NAME appenv)
	install(TARGETS appenv_shared DESTINATION lib)
endif()

if(${BUILD_STATIC})
	add_library(appenv_static STATIC ${SOURCE})
	set_target_properties(appenv_static PROPERTIES OUTPUT_NAME appenv)
	install(TARGETS appenv_static DESTINATION lib)
endif()

install(DIRECTORY include/ DESTINATION include/)

IF(WIN32)
	if(${BUILD_SHARED)
		target_compile_definitions(appenv_shared PUBLIC -DWINBUILD)
	endif()
	if(${BUILD_STATIC})
		target_compile_definitions(appenv_static PUBLIC -DWINBUILD)
	endif()
ENDIF()

