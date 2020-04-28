# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB ROOT_HEADER "../include/hypatia/*.h")
file(GLOB ROOT_SOURCE "./*.cpp")

file(GLOB COORD_HEADER "../include/hypatia/coordinates/*.h")
file(GLOB COORD_SOURCE "./coordinates/*.cpp")

file(GLOB MODEL_HEADER "../include/hypatia/models/*.h")
file(GLOB MODEL_SOURCE "./models/*.cpp")

file(GLOB PRIMI_HEADER "../include/hypatia/primitives/*.h")
file(GLOB PRIMI_SOURCE "./primitives/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(hypatia ${ROOT_SOURCE} ${COORD_SOURCE} ${MODEL_SOURCE} ${PRIMI_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(hypatia PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${ROOT_HEADER})

install(TARGETS hypatia DESTINATION lib)
install(FILES ${ROOT_HEADER} DESTINATION include/hypatia)
install(FILES ${COORD_HEADER} DESTINATION include/hypatia/coordinates)
install(FILES ${MODEL_HEADER} DESTINATION include/hypatia/models)
install(FILES ${PRIMI_HEADER} DESTINATION include/hypatia/primitives)