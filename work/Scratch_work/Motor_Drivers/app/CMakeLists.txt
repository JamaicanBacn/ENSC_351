include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable( Motor_Drivers ${MY_SOURCES})

target_link_libraries( Motor_Drivers LINK_PRIVATE hal)

#target_link_libraries( Motor_Drivers PRIVATE gpiod m)

add_custom_command(TARGET Motor_Drivers POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:Motor_Drivers>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")