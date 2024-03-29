cmake_minimum_required(VERSION 3.2)
project(hilma
    VERSION 0.2
    DESCRIPTION "library for 2D/3D geometric"
    LANGUAGES CXX)

# The compiled library code is here
add_subdirectory(src)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely suppor"${PROJECT_SOURCE_DIR}/src/*.cu"t folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
	add_subdirectory(docs)
    else()
	message(STATUS "Doxygen not found, not building docs")
    endif()

    add_subdirectory(examples)
endif()



