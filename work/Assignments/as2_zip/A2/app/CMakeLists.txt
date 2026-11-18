include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable( lightSampler ${MY_SOURCES})

target_link_libraries( lightSampler LINK_PRIVATE hal)

target_link_libraries( lightSampler PRIVATE gpiod m)

add_custom_command(TARGET lightSampler POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:lightSampler>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")