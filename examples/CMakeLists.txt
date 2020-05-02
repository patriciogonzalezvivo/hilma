add_executable (plane plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (plane PRIVATE hilma)

add_executable (points_plane points_plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (points_plane PRIVATE hilma)

add_executable (load load.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (load PRIVATE hilma)

add_executable (rect rect.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (rect PRIVATE hilma)