include_directories(include)
find_package(ALSA REQUIRED)

file(GLOB MY_SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable( beatbox ${MY_SOURCES})

target_link_libraries(beatbox PRIVATE hal asound gpiod m)

set(DEPLOY_DIR "$ENV{HOME}/ensc351/public/myApps")
add_custom_target(deploy ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${DEPLOY_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:beatbox>" "${DEPLOY_DIR}/beatbox"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/wave-files" "${DEPLOY_DIR}/beatbox-wav-files"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/as3-server" "${DEPLOY_DIR}/beatbox-server-copy"
    DEPENDS beatbox
    COMMENT "Deploying beatbox, WAV files and web server to ${DEPLOY_DIR}"
    VERBATIM
)
