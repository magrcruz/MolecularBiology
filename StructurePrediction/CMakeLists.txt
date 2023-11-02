# CMakeList.txt : CMake project for MolecularBiology, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(StructurePrediction  "StructurePrediction.cpp" "StructurePrediction.h")
#configure_file(${CMAKE_SOURCE_DIR}/StructurePrediction/BRCA1.txt ${CMAKE_BINARY_DIR}/StructurePrediction/BRCA1.txt COPYONLY)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET StructurePrediction PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
