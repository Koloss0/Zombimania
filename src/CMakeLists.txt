string(TIMESTAMP TIMESTAMP_UTC UTC)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# check if configuration file was generated successfully
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/config.h)
    message(FATAL_ERROR "Failed to generate config.h file!")
endif()

add_executable(zombimania)

target_sources(zombimania PRIVATE config.c)

add_subdirectory(core)
add_subdirectory(gfx)
add_subdirectory(io)
add_subdirectory(math)
add_subdirectory(states)

target_include_directories(zombimania PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(zombimania PRIVATE "$<$<COMPILE_LANGUAGE:C>:${c_flags}>")