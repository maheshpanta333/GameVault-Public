include(FetchContent)

# Raylib: try system first, then fetch if missing
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    message(STATUS "Raylib not found on system, fetching with FetchContent...")
    set(RAYLIB_VERSION 5.5)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
    )
    
    # Configure raylib build options FOR WINDOWS
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    
    # Windows-specific raylib configuration
    if (WIN32)
        set(SUPPORT_X11 OFF CACHE BOOL "" FORCE)
        set(USE_EXTERNAL_GLFW OFF CACHE BOOL "" FORCE)
        set(PLATFORM "Desktop" CACHE STRING "" FORCE)
    endif()
    
    FetchContent_MakeAvailable(raylib)
else()
    message(STATUS "Using system raylib")
endif()
# Raygui: header-only, check for system installation first
find_path(RAYGUI_INCLUDE_DIR
    NAMES raygui.h
    PATHS /usr/include /usr/local/include ${CMAKE_SOURCE_DIR}/include
)
if (NOT RAYGUI_INCLUDE_DIR)
    message(STATUS "Raygui not found on system, fetching with FetchContent...")
    FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(raygui)
    set(raygui_SOURCE_DIR ${raygui_SOURCE_DIR}/src)
else()
    message(STATUS "Using system raygui from: ${RAYGUI_INCLUDE_DIR}")
    set(raygui_SOURCE_DIR ${RAYGUI_INCLUDE_DIR})
endif()

# Create exactly one raygui INTERFACE library
add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR})

# ENet: try system first, then fetch if missing
find_package(ENet QUIET)
if (NOT ENet_FOUND)
    message(STATUS "ENet not found on system, fetching with FetchContent...")
    FetchContent_Declare(
        enet
        GIT_REPOSITORY https://github.com/lsalzman/enet.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(enet)
    add_library(ENet::ENet ALIAS enet)
else()
    message(STATUS "Using system ENet")
endif()

# Platform-specific
if (WIN32)
    add_compile_definitions(PLATFORM_DESKTOP)
    if (TARGET enet AND NOT TARGET ENet::ENet)
        target_link_libraries(enet INTERFACE ws2_32 winmm)
    endif()
endif()

# Verify
if (NOT TARGET raylib)
    message(FATAL_ERROR "Raylib target not available")
endif()
if (NOT TARGET raygui)
    message(FATAL_ERROR "Raygui target not available")
endif()
if (NOT TARGET ENet::ENet)
    message(FATAL_ERROR "ENet target not available")
endif()

message(STATUS "All dependencies resolved successfully")
