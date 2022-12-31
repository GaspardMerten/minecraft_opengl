#Template for INFOH502
#Heavily inspired from https://github.com/Polytonic/Glitter/blob/master/CMakeLists.txt

#project setup
project(openglproject)

#set the minimal version of cmake and the minimal version of the c++ language
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(OpenGL REQUIRED)

#for glad library
add_library( glad STATIC 3rdParty/glad/src/glad.c src/objects/game_object/GameObject.cpp src/objects/game_object/GameObject.h src/texture/texture/Texture.cpp src/texture/texture/Texture.h)
set(GLAD_INCLUDE "3rdParty/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

#for GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdParty/glfw)



include_directories(3rdParty/glad/include/
        3rdParty/glfw/include/
        3rdParty/glm/
        3rdParty/stb/)


add_executable(main src/main.cpp "src/utils/shader/load/load_shader.cpp" src/utils/shader/load/load_shader.h  src/objects/transform/Transform.cpp src/objects/transform/Transform.h src/utils/shader/shader/Shader.cpp src/utils/shader/shader/Shader.h src/objects/mesh/Mesh.cpp src/objects/mesh/Mesh.h src/objects/renderer/Renderer.cpp src/objects/renderer/Renderer.h src/utils/shader/shader/Light.h src/objects/camera/Camera.cpp  src/world/World.h src/world/World.cpp  src/utils/world/generate_world.h src/utils/world/generate_world.cpp src/controls/player_controls/PlayerControls.cpp src/controls/player_controls/PlayerControls.h src/utils/shader/shader/Light.cpp   src/texture/texture/Texture.h src/texture/texture/Texture.cpp src/texture/manager/TextureManager.cpp src/texture/manager/TextureManager.h src/controls/camera/CameraControls.cpp src/controls/camera/CameraControls.h src/game/Minecraft.cpp src/game/Minecraft.h src/objects/mesh/manager/MeshManager.h src/objects/mesh/manager/MeshManager.cpp)
#Specify which libraries you want to use with your executable
target_link_libraries(main PUBLIC OpenGL::GL glfw glad)