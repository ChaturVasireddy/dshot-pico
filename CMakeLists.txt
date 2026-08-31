add_library(dshot STATIC
    dshot.c
)

target_include_directories(dshot PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_generate_pio_header(dshot
    ${CMAKE_CURRENT_LIST_DIR}/dshot.pio
)

target_link_libraries(dshot PUBLIC
    pico_stdlib
    hardware_pio
)