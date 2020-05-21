include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable (rect rect.cpp)
target_link_libraries (rect PRIVATE hilma)

add_executable (gener gener.cpp)
target_link_libraries (gener PRIVATE hilma)

add_executable (extrude extrude.cpp)
target_link_libraries (extrude PRIVATE hilma)

add_executable (spline spline.cpp)
target_link_libraries (spline PRIVATE hilma)

add_executable (tube tube.cpp)
target_link_libraries (tube PRIVATE hilma)

add_executable (ring ring.cpp)
target_link_libraries (ring PRIVATE hilma)

add_executable (cornell cornell.cpp)
target_link_libraries (cornell PRIVATE hilma)

add_executable (raytracer raytracer.cpp)
target_link_libraries (raytracer PRIVATE hilma)

add_executable (terrain terrain.cpp)
target_link_libraries (terrain PRIVATE hilma)

add_executable (load load.cpp)
target_link_libraries (load PRIVATE hilma)
