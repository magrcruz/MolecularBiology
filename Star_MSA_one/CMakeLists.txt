# CMakeList.txt : CMake project for MolecularBiology, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(Star_MSA "Star_MSA.cpp" "Star_MSA.h" "Alignments.h" "Utils.h" )
#configure_file(${CMAKE_SOURCE_DIR}/Star_MSA/Sequencias.txt ${CMAKE_BINARY_DIR}/Star_MSA/Sequencias.txt COPYONLY)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Star_MSA PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

#alinear primero f con f, luego r con r y luego todos contra todos