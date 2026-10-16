include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable( IR_test ${MY_SOURCES})

target_link_libraries( IR_test LINK_PRIVATE hal)
target_link_libraries( IR_test PRIVATE gpiod)

add_custom_command(TARGET IR_test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:IR_test>"
        "$ENV{HOME}/public/"
    COMMENT "Copying exec to public")