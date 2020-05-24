

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

Project(Sandbox)

add_executable( Sandbox
    src/Area2D.cpp
    src/Area2D.h
    src/Ball.h
    src/Sandbox2D.cpp
    src/Sandbox2D.h
    src/SandboxApp.cpp
)

target_compile_features(Sandbox PRIVATE cxx_std_17)

target_link_libraries(Sandbox Marble)


if(UNIX)
    target_link_libraries(Sandbox dl pthread stdc++fs)

    add_custom_command(
        TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:Sandbox>/assets/
    )
    #target_compile_definitions(Sandbox PUBLIC SANDBOX_ASSETS_PATH=$<TARGET_FILE_DIR:Sandbox>)

elseif(WIN32)
    target_link_libraries(Sandbox opengl32.lib)
endif()

