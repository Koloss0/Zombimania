cmake_minimum_required(VERSION 3.15)

project(Zombimania
	VERSION 0.1.0
	DESCRIPTION "Unleash mayhem in an online co-op, undead-infested, post-apocalyptic shooter"
	LANGUAGES C)

set(c_flags -g -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wconversion -Wsign-conversion -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=5 -Wswitch-default -Wundef -Wreturn-type -Wchar-subscripts -Wcomment -Wunused -Wno-unused-parameter)

add_subdirectory(src bin)
add_subdirectory(deps)