# cmake_minimum_required(VERSION 2.8)
# cmake_policy(SET CMP0015 NEW)
# project(boids)
# set(CMAKE_VERBOSE_MAKEFILE on)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build.emscripten)
# include_directories("lib/assimp/include")
# link_directories("lib/assimp/lib-js")
# link_libraries("assimp")

# add_definitions("-O3 -flto --closure 1 
# 				-s USE_PTHREADS=1 
# 				-s PTHREAD_POOL_SIZE=16 
# 				-s USE_GLFW=3 
# 				-s MAX_WEBGL_VERSION=2 
# 				-s MIN_WEBGL_VERSION=2 
# 				-s TOTAL_MEMORY=128MB 
# 				--bind
# 				--preload-file ${CMAKE_SOURCE_DIR}/src/shaders")
# add_executable(client ${CORE_SRC} ${CORE_HDR})

cmake_minimum_required(VERSION 3.16)
project(index)
# add_executable(index test.cpp)
# file(GLOB CORE_HDR ../src/headers/.h)
# file(GLOB CORE_HDR ../src/headers/.hpp)
# file(GLOB CORE_SRC ../src/.cpp)
# add_executable(index ${CORE_SRC} ${CORE_HDR})
add_executable(index 
					"../src/headers/boid.hpp"
					"../src/headers/jsonboid.hpp"
					"../src/headers/level.hpp"
					"../src/headers/render.hpp"
					"../src/headers/rgb.hpp"
					

					"../src/boid.cpp"
					"../src/globals.cpp"
					"../src/jsonboid.cpp"
					"../src/level.cpp"
					"../src/main.cpp"
					"../src/render.cpp"
					"../src/rgb.cpp"
					
)
target_precompile_headers(index PRIVATE "src/headers/globals.hpp" "src/headers/v2d.h" "src/headers/settings.hpp")
set_target_properties(index PROPERTIES COMPILE_FLAGS "-std=c++17 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=16 -O3 -fexceptions")
set_target_properties(index PROPERTIES LINK_FLAGS "
						-std=c++17 
						-O3
						-flto 
						-s ASSERTIONS=1
						
						-s USE_GLFW=3 
						-s MAX_WEBGL_VERSION=2 
						-s MIN_WEBGL_VERSION=2 
						-s WASM=1
						-s TOTAL_MEMORY=256MB 
						-s USE_PTHREADS=1
						-s PTHREAD_POOL_SIZE=16
						--bind
						--preload-file ../src/shaders")