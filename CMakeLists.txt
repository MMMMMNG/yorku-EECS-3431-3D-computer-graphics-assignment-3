cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.
project(raytrace VERSION 0.1.0 LANGUAGES C CXX)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

add_executable(raytrace raytrace.cpp ray_utils.cpp)
target_link_libraries(raytrace PRIVATE glm::glm)
