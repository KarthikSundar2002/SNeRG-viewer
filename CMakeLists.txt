cmake_minimum_required(VERSION 3.12)
project(snerg_viewer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL Package
find_package(OpenGL REQUIRED)
set(DEPENDENCIES OpenGL::GL)

# Adding GLFW package
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw")
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW example" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW tests" FORCE)
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW docs" FORCE)
add_subdirectory(${GLFW_DIR})
set(DEPENDENCIES ${DEPENDENCIES} glfw)
include_directories(${GLFW_DIR}/include)

# Adding GLAD package
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad")
add_subdirectory("${GLAD_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_mx_43 API gl:core=4.3)
set(DEPENDENCIES ${DEPENDENCIES} glad_gl_core_mx_43)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/gladsources/glad_gl_core_mx_43/include")
message("${CMAKE_CURRENT_BINARY_DIR}/gladsources/glad_gl_core_mx_43/include")

# Add GLM package
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm")
include_directories("${GLM_DIR}")

# Add stb_image package
set(STB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb")
include_directories(${STB_PATH})

# Add nlohmann_json package
set(NLOHMANN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json/single_include")
include_directories(${NLOHMANN_PATH})

# Adding source files
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# building and linking
add_executable(snerg_viewer main.cpp ${SOURCE_FILES})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_compile_options(snerg_viewer PRIVATE "/WX")
else()
	target_compile_options(snerg_viewer PRIVATE "-Werror")
endif()
target_link_libraries(snerg_viewer PUBLIC ${DEPENDENCIES})