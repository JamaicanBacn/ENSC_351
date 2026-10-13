include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable( LED_USB ${MY_SOURCES})

target_link_libraries( LED_USB LINK_PRIVATE hal)

target_link_libraries( LED_USB PRIVATE gpiod m)

add_custom_command(TARGET LED_USB POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:LED_USB>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")