# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB ROOT_HEADER "../include/hypatia/*.h")
file(GLOB ROOT_SOURCE "./*.cpp")

file(GLOB IO_HEADER "../include/hypatia/io/*.h")
file(GLOB IO_SOURCE "./io/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(hilma ${ROOT_SOURCE} ${IO_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(hilma PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${ROOT_HEADER})

install(TARGETS hilma DESTINATION lib)
install(FILES ${ROOT_HEADER} DESTINATION include/hilma)
install(FILES ${IO_HEADER} DESTINATION include/hilma/io)