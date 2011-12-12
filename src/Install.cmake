# Targets
install (
    TARGETS corkscrew2
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Prerequisites
#include (InstallRequiredSystemLibraries)

#set (EXECUTABLE_PATH "${CMAKE_INSTALL_PREFIX}/bin/klsserver${CMAKE_EXECUTABLE_SUFFIX}")
#set (MY_BUNDLE_UTILITIES_PATH "${PROJECT_SOURCE_DIR}/3rdparty/cmake/MyBundleUtilities.cmake")
#configure_file(
#    "${PROJECT_SOURCE_DIR}/3rdparty/cmake/FixBundle.cmake.in"
#    "${PROJECT_BINARY_DIR}/FixBundle.cmake"
#    @ONLY
#)

#install (SCRIPT "${PROJECT_BINARY_DIR}/FixBundle.cmake")
