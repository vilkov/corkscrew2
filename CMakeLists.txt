cmake_minimum_required (VERSION 2.8)
project (corkscrew2)


# Main information
if (CMAKE_CROSSCOMPILING)
    message (STATUS "Cross compiling to: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
else ()
    message (STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
endif ()
message (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})
if (MSVC)
	message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
	add_definitions (/W3)
elseif (BORLAND)
	message(STATUS "Compiler: BCC")
elseif (CMAKE_COMPILER_IS_GNUCXX) 
	message(STATUS "Compiler: GNUCXX")
    add_definitions (-Wall)
#    add_definitions (-Wall -pedantic)
elseif (CMAKE_COMPILER_IS_GNUC) 
	message(STATUS "Compiler: GNUC")
    add_definitions (-Wall)
#    add_definitions (-Wall -pedantic)
else () 
	message (STATUS "Unknown compiler")
endif ()
message (STATUS "CMake generates " ${CMAKE_GENERATOR})

# Main settings
set (USE_GPROF    NO CACHE BOOL "Use GProf profiler?")
set (UNIT_TESTING NO CACHE BOOL "Compile binary executable for unit testing?")
set (PROJECT_GENERATED_FILES_DIR "${PROJECT_BINARY_DIR}/generated")

# Custom modules
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Fix path delimeters
if (WIN32)
    file (TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX)
endif ()

# Includes

# Link directories

# Libs
if (UNIX)
	set (common_LIBS ${common_LIBS} dl)
endif ()

if (USE_GPROF)
	set (common_LIBS ${common_LIBS} gmon)
endif (USE_GPROF)

# Definitions
add_definitions (-DUNICODE)

# We're require at least Windows 2000 (http://msdn.microsoft.com/en-us/library/Aa383745)
if (WIN32)
    add_definitions (-D_WIN32_WINNT=0x0500 -DWINVER=0x0500)
endif ()


# Targets
add_subdirectory ("src")


# GProf flags
if (USE_GPROF)
    set (CMAKE_CXX_FLAGS        ${CMAKE_CXX_FLAGS}        -pg)
    set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -pg)
endif (USE_GPROF)
