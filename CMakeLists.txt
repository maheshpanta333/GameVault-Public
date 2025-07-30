cmake_minimum_required(VERSION 3.22)
project(GameVault LANGUAGES CXX RC)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (WIN32)
    add_compile_definitions(PLATFORM_DESKTOP)
    set(PLATFORM "PLATFORM_DESKTOP" CACHE STRING "" FORCE)
endif()
# Default to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Enable automatic RC file handling
set(CMAKE_AUTORCC ON)

# Put all executables in bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch or find dependencies
include(cmake/dependencies.cmake)

# Copy shared assets into bin/assets at build time
add_custom_command(
    OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
    COMMENT "Copying assets to build/bin/assets"
)
add_custom_target(copy_assets ALL
    DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
)

# Build RayGui implementation exactly once
add_library(raygui_impl STATIC
    ${CMAKE_SOURCE_DIR}/raygui_impl.cpp
)
target_include_directories(raygui_impl PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${raygui_SOURCE_DIR}
)
target_link_libraries(raygui_impl PUBLIC
    raylib
)

# Collect launcher sources
set(LAUNCHER_SOURCES
    ${CMAKE_SOURCE_DIR}/launcher.cpp
    ${CMAKE_SOURCE_DIR}/theme.cpp
    ${CMAKE_SOURCE_DIR}/startgame.cpp
    ${CMAKE_SOURCE_DIR}/core_var.cpp
)

# On Windows, embed the icon resource and mark as GUI app
if (WIN32)
    list(APPEND LAUNCHER_SOURCES
        ${CMAKE_SOURCE_DIR}/build_resources/icon.rc
    )
    add_executable(GameVault WIN32 ${LAUNCHER_SOURCES})
else()
    add_executable(GameVault ${LAUNCHER_SOURCES})
endif()

target_include_directories(GameVault PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${raygui_SOURCE_DIR}
)
target_link_libraries(GameVault PRIVATE
    raylib
    raygui
    raygui_impl
    ENet::ENet
)
if (WIN32)
    target_link_libraries(GameVault PRIVATE
        opengl32
        gdi32
        winmm
        ws2_32
    )
endif()

add_dependencies(GameVault copy_assets)

# Add each game and the multiplayer client/server
add_subdirectory(pingpong)
add_subdirectory(Shrink_Shot)
add_subdirectory(snake)
add_subdirectory(space_invaders)
add_subdirectory(multiplayer)
