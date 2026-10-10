cmake_minimum_required(VERSION 3.16)
project(MyRaylibProject C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Fix for the CMP0135 warning in your logs
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Automatically download and include raylib using FetchContent
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        6.0 # Or use a specific release tag like 5.0, 5.5, etc.
)

# Build raylib as part of the project (uses correct compiler tools automatically)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

FILE(GLOB SRCFILES src/*.c)
# Define your executable
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link raylib and system dependencies based on the OS
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(TARGET raylib)
    if(APPLE)
        # MacOS specific framework dependencies
        target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    elseif(WIN32)
        # Windows specific system dependencies
        target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32 opengl32)
    endif()
endif()
