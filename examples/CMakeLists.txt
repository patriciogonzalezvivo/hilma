include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable (rect rect.cpp)
target_link_libraries (rect PRIVATE hilma)

add_executable (plane plane.cpp)
target_link_libraries (plane PRIVATE hilma)

add_executable (points_plane points_plane.cpp)
target_link_libraries (points_plane PRIVATE hilma)

add_executable (box box.cpp)
target_link_libraries (box PRIVATE hilma)

add_executable (sphere sphere.cpp)
target_link_libraries (sphere PRIVATE hilma)

add_executable (icosphere icosphere.cpp)
target_link_libraries (icosphere PRIVATE hilma)

add_executable (cylinder cylinder.cpp)
target_link_libraries (cylinder PRIVATE hilma)

add_executable (cone cone.cpp)
target_link_libraries (cone PRIVATE hilma)

add_executable (surface surface.cpp)
target_link_libraries (surface PRIVATE hilma)

add_executable (extrude extrude.cpp)
target_link_libraries (extrude PRIVATE hilma)

add_executable (spline spline.cpp)
target_link_libraries (spline PRIVATE hilma)

add_executable (tube tube.cpp)
target_link_libraries (tube PRIVATE hilma)

add_executable (load load.cpp)
target_link_libraries (load PRIVATE hilma)
