# CMakeList.txt : CMake project for MolecularBiology, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(SmithWaterman "SmithWaterman.cpp" "Alignments.h" "SmithWaterman.h" "Utils.h")
configure_file(${CMAKE_SOURCE_DIR}/SmithWaterman/Sequencias.txt ${CMAKE_BINARY_DIR}/SmithWaterman/Sequencias.txt COPYONLY)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SmithWaterman PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
