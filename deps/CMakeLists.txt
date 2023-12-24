add_subdirectory(glad)
add_subdirectory(glfw)

target_link_libraries(zombimania PRIVATE
	glfw
	glad
)
