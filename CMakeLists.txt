# CMakeList.txt : CMake project for FinalProject-040585687, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("FinalProject-040585687")


# Add source to this project's executable.
add_executable (FinalProject-040585687 "FinalProject-040585687.cpp")

target_include_directories(FinalProject-040585687 PUBLIC "./include")
target_link_directories(FinalProject-040585687 PUBLIC "./lib")
target_link_libraries(FinalProject-040585687 sfml-main-d sfml-graphics-d sfml-system-d sfml-window-d tgui-d)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FinalProject-040585687 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
