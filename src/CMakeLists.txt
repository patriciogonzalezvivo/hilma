file(GLOB ROOT_HEADER "${PROJECT_SOURCE_DIR}/include/hilma/*.h")
file(GLOB ROOT_SOURCE "${PROJECT_SOURCE_DIR}/src/*.cpp")

file(GLOB IO_HEADER "${PROJECT_SOURCE_DIR}/include/hilma/io/*.h")
file(GLOB IO_SOURCE "${PROJECT_SOURCE_DIR}/src/io/*.cpp")

file(GLOB OPS_HEADER "${PROJECT_SOURCE_DIR}/include/hilma/ops/*.h")
file(GLOB OPS_SOURCE "${PROJECT_SOURCE_DIR}/src/ops/*.cpp")

file(GLOB TYPES_HEADER "${PROJECT_SOURCE_DIR}/include/hilma/types/*.h")
file(GLOB TYPES_SOURCE "${PROJECT_SOURCE_DIR}/src/types/*.cpp")

set(INCLUDE_FOLDER "${PROJECT_SOURCE_DIR}/include")
set(INCLUDE_SWIG_FILE "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.i")

# C++ LIBRAY
#
add_library(hilma ${ROOT_SOURCE} ${IO_SOURCE} ${OPS_SOURCE} ${TYPES_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(hilma PUBLIC ${INCLUDE_FOLDER})

# IDEs should put the headers in a nice place
source_group(   TREE ${INCLUDE_FOLDER} 
                PREFIX "Header Files" FILES ${ROOT_HEADER})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${ROOT_HEADER} DESTINATION include/hilma)
install(FILES ${IO_HEADER} DESTINATION include/hilma/io)
install(FILES ${OPS_HEADER} DESTINATION include/hilma/ops)
install(FILES ${TYPES_HEADER} DESTINATION include/hilma/types)



# PYTHON BINDINGS 
#
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 OLD)
endif(POLICY CMP0078)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

if(NOT PYTHON_NUMPY_INCLUDE_DIR)
    execute_process(COMMAND python3 -c "import numpy; print(numpy.get_include())"
                    RESULT_VARIABLE PYTHON_NUMPY_PROCESS
                    OUTPUT_VARIABLE PYTHON_NUMPY_INCLUDE_DIR
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(PYTHON_NUMPY_PROCESS EQUAL 0)
        file(TO_CMAKE_PATH "${PYTHON_NUMPY_INCLUDE_DIR}" PYTHON_NUMPY_INCLUDE_CMAKE_PATH)
        set(PYTHON_NUMPY_INCLUDE_DIR ${PYTHON_NUMPY_INCLUDE_CMAKE_PATH} CACHE PATH "Numpy include directory")
    else()
        message(SEND_ERROR "Could not determine the NumPy include directory, verify that NumPy was installed correctly.")
    endif()
endif()

include_directories(${PYTHON_NUMPY_INCLUDE_DIR} ${INCLUDE_FOLDER})
set_property(SOURCE ${INCLUDE_SWIG_FILE} PROPERTY CPLUSPLUS ON)

if (CMAKE_VERSION VERSION_LESS "3.8")
    swig_add_module(${PROJECT_NAME} 
                    python 
                    ${INCLUDE_SWIG_FILE} ${ROOT_SOURCE} ${IO_SOURCE} ${OPS_SOURCE} ${TYPES_SOURCE})
else()
    swig_add_library(${PROJECT_NAME} 
                    LANGUAGE python
                    SOURCES ${INCLUDE_SWIG_FILE} ${ROOT_SOURCE} ${IO_SOURCE} ${OPS_SOURCE} ${TYPES_SOURCE})
endif()
swig_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

# INSTALL PYTHON BINDINGS
# Get the python site packages directory by invoking python
execute_process(COMMAND python3 -c "import site; print(site.getsitepackages()[0])" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
message("Installing at ${SITE_PACKAGES}")

install(TARGETS _${PROJECT_NAME} DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py DESTINATION ${PYTHON_SITE_PACKAGES})
