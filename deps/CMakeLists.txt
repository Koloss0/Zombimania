add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(miniaudio)

target_link_libraries(zombimania PRIVATE
	glfw
	glad
)
