# CMakeList.txt : CMake project for MolecularBiology, include source and define
# project specific logic here.
#

add_executable(Profiles "Profiles.cpp" "Profiles.h" )


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Profiles PROPERTY CXX_STANDARD 20)
endif()

