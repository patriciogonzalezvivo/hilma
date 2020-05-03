add_executable (rect rect.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (rect PRIVATE hilma)

add_executable (plane plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (plane PRIVATE hilma)

add_executable (points_plane points_plane.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (points_plane PRIVATE hilma)

add_executable (box box.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (box PRIVATE hilma)

add_executable (sphere sphere.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (sphere PRIVATE hilma)

add_executable (icosphere icosphere.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (icosphere PRIVATE hilma)

add_executable (surface surface.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (surface PRIVATE hilma)

add_executable (extrude extrude.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (extrude PRIVATE hilma)

add_executable (load load.cpp)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries (load PRIVATE hilma)
