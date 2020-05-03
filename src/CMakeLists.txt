# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB ROOT_HEADER "../include/hypatia/*.h")
file(GLOB ROOT_SOURCE "./*.cpp")

file(GLOB IO_HEADER "../include/hypatia/io/*.h")
file(GLOB IO_SOURCE "./io/*.cpp")

file(GLOB OPS_HEADER "../include/hypatia/ops/*.h")
file(GLOB OPS_SOURCE "./ops/*.cpp")

file(GLOB TYPES_HEADER "../include/hypatia/types/*.h")
file(GLOB TYPES_SOURCE "./types/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(hilma ${ROOT_SOURCE} ${IO_SOURCE} ${OPS_SOURCE} ${TYPES_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(hilma PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${ROOT_HEADER})

install(TARGETS hilma DESTINATION lib)
install(FILES ${ROOT_HEADER} DESTINATION include/hilma)
install(FILES ${IO_HEADER} DESTINATION include/hilma/io)
install(FILES ${OPS_HEADER} DESTINATION include/hilma/ops)
install(FILES ${TYPES_HEADER} DESTINATION include/hilma/types)

# find_package(SWIG REQUIRED)
# include_directories(${PYTHON_INCLUDE_DIRS})
# find_package(Python REQUIRED COMPONENTS Interpreter Development)
# include(${SWIG_USE_FILE})
# set_property(SOURCE ../hilma.i PROPERTY CPLUSPLUS ON)
# swig_add_library(   hilma 
#                     TYPE MODULE 
#                     LANGUAGE python 
#                     SOURCES ../hilma.i ${ROOT_SOURCE} ${IO_SOURCE} ${OPS_SOURCE} ${TYPES_SOURCE})
# swig_link_libraries(hilma ${PYTHON_LIBRARIES})

# # INSTALL PYTHON BINDINGS
# # Get the python site packages directory by invoking python
# execute_process(COMMAND python3 -c "import site; print(site.getsitepackages()[0])" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
# message("PYTHON_SITE_PACKAGES = ${PYTHON_SITE_PACKAGES}")
# install(TARGETS _hilma DESTINATION ${PYTHON_SITE_PACKAGES})
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hilma.py DESTINATION ${PYTHON_SITE_PACKAGES})
