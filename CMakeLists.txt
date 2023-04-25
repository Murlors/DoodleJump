cmake_minimum_required(VERSION 3.0)

project(DoodleJump)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")

set(CMAKE_EXE_LINKER_FLAGS "-static -static-libstdc++")

include_directories(include)
link_directories(lib)

add_executable(DoodleJump
        src/DoodleJump.cpp
        src/Land.cpp
        src/Player.cpp
        src/SaveData.cpp
        src/Scene.cpp
        src/Welcome.cpp)

target_link_libraries(DoodleJump logo64 gdiplus winmm)