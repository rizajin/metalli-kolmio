cmake_minimum_required(VERSION 3.21)

project(metalli-kolmio LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OBJCXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_OSX_ARCHITECTURES "x86_64")

add_subdirectory(glfw)

find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
find_library(METAL_FRAMEWORK Metal REQUIRED)
find_library(APPKIT_FRAMEWORK AppKit REQUIRED)
find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)

add_executable(MetalliKolmio main.cpp win.mm)
target_include_directories(MetalliKolmio PUBLIC metal-cpp)
target_include_directories(MetalliKolmio PUBLIC glfw/include)
target_link_libraries(MetalliKolmio
    ${COCOA_FRAMEWORK}
    ${METAL_FRAMEWORK}
    ${APPKIT_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
    glfw
)

set(SHADER_SRC_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/shaders)

file(MAKE_DIRECTORY ${SHADER_OUT_DIR})

set(METAL_SHADER_SOURCE ${SHADER_SRC_DIR}/shaders.metal)
set(METAL_SHADER_AIR ${SHADER_OUT_DIR}/shaders.air)
set(METAL_SHADER_LIB ${SHADER_OUT_DIR}/shaders.metallib)

add_custom_command(
    OUTPUT ${METAL_SHADER_AIR}
    COMMAND xcrun -sdk macosx metal -c ${METAL_SHADER_SOURCE} -o ${METAL_SHADER_AIR}
    DEPENDS ${METAL_SHADER_SOURCE}
    COMMENT "Compiling Metal shader to AIR"
)

add_custom_command(
    OUTPUT ${METAL_SHADER_LIB}
    COMMAND xcrun -sdk macosx metallib ${METAL_SHADER_AIR} -o ${METAL_SHADER_LIB}
    DEPENDS ${METAL_SHADER_AIR}
    COMMENT "Converting AIR to Metal library"
)

add_custom_target(
    MetalShaders ALL DEPENDS ${METAL_SHADER_LIB}
)

add_dependencies(MetalliKolmio MetalShaders)

install(FILES ${METAL_SHADER_LIB} DESTINATION ${CMAKE_BINARY_DIR})
