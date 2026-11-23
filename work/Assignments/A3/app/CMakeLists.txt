include_directories(include)
find_package(ALSA REQUIRED)

file(GLOB MY_SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable( beatbox ${MY_SOURCES})

target_link_libraries(beatbox LINK_PRIVATE hal asound)
target_link_libraries(beatbox PRIVATE gpiod m)

set(NFS_DIR "$ENV{HOME}/public")

add_custom_command(TARGET beatbox POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
            "$<TARGET_FILE:beatbox>" "${NFS_DIR}/beatbox"

    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/wave-files" "${NFS_DIR}/wave-files"

    COMMENT "Copying beatbox and wave files to NFS"
    VERBATIM
)