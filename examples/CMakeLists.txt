include_directories("${CMAKE_SOURCE_DIR}/include")

if(POLICY CMP0002)
  cmake_policy(SET CMP0002 OLD)
endif(POLICY CMP0002)

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

add_executable (cornell cornell.cpp)
target_link_libraries (cornell PRIVATE hilma)

add_executable (raytracer raytracer.cpp)
target_link_libraries (raytracer PRIVATE hilma)

add_executable (raytracer_lines raytracer_lines.cpp)
target_link_libraries (raytracer_lines PRIVATE hilma)

add_executable (terrain terrain.cpp)
target_link_libraries (terrain PRIVATE hilma)

add_executable (load load.cpp)
target_link_libraries (load PRIVATE hilma)

add_executable (process process.cpp)
target_link_libraries (process PRIVATE hilma)

add_executable (atlas atlas.cpp)
target_link_libraries (atlas PRIVATE hilma)
