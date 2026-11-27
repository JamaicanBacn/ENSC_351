include_directories(include)
find_package(ALSA REQUIRED)

file(GLOB MY_SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable( beatbox ${MY_SOURCES})

target_link_libraries(beatbox PRIVATE hal asound gpiod m)


add_custom_command(TARGET beatbox POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:beatbox>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")

add_custom_command(TARGET beatbox POST_BUILD 
COMMAND "${CMAKE_COMMAND}" -E copy_directory
"${CMAKE_SOURCE_DIR}/wave-files"
"~/public/wave-files" 
COMMENT "Copying WAVE files to public NFS directory")
