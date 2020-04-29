
add_executable (plane plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (plane PRIVATE hilma)