include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable( Joystick ${MY_SOURCES})

target_link_libraries( Joystick LINK_PRIVATE hal)

target_link_libraries( Joystick PRIVATE gpiod m)

add_custom_command(TARGET Joystick POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:Joystick>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")