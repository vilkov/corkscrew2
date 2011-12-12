# 3rdparty


# CMake tools
include ("collect_sources")


# Tools defines
add_definitions (
    -DTOOLS_THREADS_BACKEND_BOOST
    -DTOOLS_POINTERS_BACKEND_BOOST
    -DTOOLS_TYPES_BACKEND_EKA
)

# Target - corkscrew2
collect_sources (corkscrew2_HEADERS corkscrew2_SOURCES)
add_executable (corkscrew2
    ${corkscrew2_SOURCES} 
)
target_link_libraries (corkscrew2
    ${common_LIBS}
)


# Install
include ("Install.cmake")
