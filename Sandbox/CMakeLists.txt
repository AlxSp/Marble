

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

Project(Sandbox)

add_library(Sandbox
    src/Area2D.cpp
    src/Area2D.h
    src/Ball.h
    src/Sandbox2D.cpp
    src/Sandbox2D.h
    src/SandboxApp.cpp
)

if(UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(Marble GL X11 Xrandr Xi dl pthread stdc++fs)
endif(UNIX)

if(WIN32)
    target_link_libraries(Marble opengl32.lib)
endif(WIN32)