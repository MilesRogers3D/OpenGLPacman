cmake_minimum_required(VERSION 3.29)
project(OpenGLPacman)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/IO/Window.h
    src/Game/Game.cpp
    src/Game/Game.h
    src/Rendering/Sprite.cpp
    src/Rendering/Sprite.h
    src/Rendering/Shader.cpp
    src/Rendering/Shader.h
    src/Rendering/Texture.cpp
    src/Rendering/Texture.h
    src/IO/ResourceManager.cpp
    src/IO/ResourceManager.h
    src/IO/Log.cpp
    src/IO/Log.h
    src/Rendering/AnimatedSprite.cpp
    src/Rendering/AnimatedSprite.h
        src/Game/Entities/Pacman.cpp
        src/Game/Entities/Pacman.h
        src/Rendering/Camera.cpp
        src/Rendering/Camera.h
)

include_directories(${PROJECT_NAME}
    src/
)

# External libraries
# ...

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

# Copy resources to build directory
# ...
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/res)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)

add_custom_command(
    OUTPUT ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} ${OUTPUT_DIR}
    DEPENDS ${RESOURCES_DIR}
    COMMENT "Copying resources to build directory..."
)

add_custom_target(copy_resources ALL DEPENDS ${OUTPUT_DIR})
add_dependencies(${PROJECT_NAME} copy_resources)

# Allow the directory to be cleaned
set_property(
    TARGET ${PROJECT_NAME}
    APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/res
)