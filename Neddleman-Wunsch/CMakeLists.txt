﻿# CMakeList.txt : CMake project for MolecularBiology, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Neddleman-Wunsch "Neddleman-Wunsch.cpp" "Neddleman-Wunsch.h")
configure_file(${CMAKE_SOURCE_DIR}/Neddleman-Wunsch/Sequencias.txt ${CMAKE_BINARY_DIR}/Neddleman-Wunsch/Sequencias.txt COPYONLY)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Neddleman-Wunsch PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
