cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

Project(Editor
        VERSION 0.1
        LANGUAGES CXX
)

add_executable( Editor
    src/AssetPath.h
    src/Archive.h
    src/Components.h
    src/EditorComponent.h
    src/EditorComponent.cpp
    src/Editor.h
    src/Editor.cpp
    src/EditorApp.cpp
)

target_compile_features(Editor PRIVATE cxx_std_17)

target_link_libraries(Editor Marble)

if(UNIX)
    target_link_libraries(Editor dl pthread stdc++fs)

    add_custom_command(
        TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:Editor>/assets/
    )
    target_compile_definitions(Editor PUBLIC SANDBOX_ASSETS_PATH="$<TARGET_FILE_DIR:Editor>")

elseif(WIN32)
    target_link_libraries(Editor opengl32.lib)

    add_custom_command(
        TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:Editor>/assets/
    )
    target_compile_definitions(Editor PUBLIC SANDBOX_ASSETS_PATH="$<TARGET_FILE_DIR:Editor>")
endif()