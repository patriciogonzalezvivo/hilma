
add_executable (points_plane points_plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (points_plane PRIVATE hilma)