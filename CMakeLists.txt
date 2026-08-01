cmake_minimum_required(VERSION 3.20)

project(static LANGUAGES C CXX)

option(STATIC_BUILD_DEMO "Build the Demo!" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(glfw3 3.3 QUIET)

add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

add_library(stb_vorbis STATIC third_party/stb/stb_vorbis.c)
target_include_directories(stb_vorbis PUBLIC third_party/stb)

add_library(static STATIC
    Include/Base/Audio.cpp
    Include/Base/Camera.cpp
    Include/Base/GameObject.cpp
    Include/Base/Text.cpp
    Include/EventHandler.cpp
    Include/Game.cpp
    Include/Render/OpenGL/GLShader.cpp
    Include/Render/OpenGL/GLTexture.cpp
    Include/Render/Renderer.cpp
    Include/Utils/Easing.cpp
    Include/Utils/Logging.cpp
    Include/Utils/TweenManager.cpp
)
add_library(static::static ALIAS static)

target_include_directories(static PUBLIC
    Include
    third_party/miniaudio
    third_party/stb
)

target_link_libraries(static PUBLIC glfw glad stb_vorbis)

target_compile_features(static PUBLIC cxx_std_20)

target_compile_options(static PRIVATE
    -Wall
    -Wextra
    $<$<CONFIG:Release>:-O3>
)

include(CheckIPOSupported)
check_ipo_supported(RESULT STATIC_IPO_SUPPORTED)
if(STATIC_IPO_SUPPORTED)
    set_target_properties(static PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
    )
endif()

if(STATIC_BUILD_DEMO)
    add_subdirectory(Demo)
endif()
