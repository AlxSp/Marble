# Marble Engine

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(
    Marble
    VERSION 0.1
    LANGUAGES CXX
)

add_library(Marble STATIC
    src/mblpch.cpp
    src/mblpch.h
    src/Marble/Core/Application.cpp
    src/Marble/Core/Application.h
    src/Marble/Core/Core.h
    src/Marble/Core/Input.cpp
    src/Marble/Core/Input.h
    src/Marble/Core/KeyCodes.h
    src/Marble/Core/Layer.cpp
    src/Marble/Core/Layer.h
    src/Marble/Core/LayerStack.cpp
    src/Marble/Core/LayerStack.h
    src/Marble/Core/Log.cpp
    src/Marble/Core/Log.h
    src/Marble/Core/MouseButtonCodes.h
    src/Marble/Core/TimeStep.h
    src/Marble/Core/Window.cpp
    src/Marble/Core/Window.h
    
    src/Marble/Debug/Instrumentor.h

    src/Marble/Events/ApplicationEvent.h
    src/Marble/Events/Event.h
    src/Marble/Events/KeyEvent.h
    src/Marble/Events/MouseEvent.h

    src/Marble/ImGui/ImGuiBuild.cpp
    src/Marble/ImGui/ImGuiLayer.cpp
    src/Marble/ImGui/ImGuiLayer.h

    src/Marble/Noise/SimplexNoise.cpp
    src/Marble/Noise/SimplexNoise.h

    src/Marble/Random/Lehmer.h

    src/Marble/Renderer/Buffer.cpp
    src/Marble/Renderer/Buffer.h
    src/Marble/Renderer/GraphicsContext.cpp
    src/Marble/Renderer/GraphicsContext.h
    src/Marble/Renderer/OrthographicCamera.cpp
    src/Marble/Renderer/OrthographicCamera.h
    src/Marble/Renderer/OrthographicCameraController.cpp
    src/Marble/Renderer/OrthographicCameraController.h
    src/Marble/Renderer/RenderCommand.cpp
    src/Marble/Renderer/RenderCommand.h
    src/Marble/Renderer/Renderer.cpp
    src/Marble/Renderer/Renderer.h
    src/Marble/Renderer/Renderer2D.cpp
    src/Marble/Renderer/Renderer2D.h
    src/Marble/Renderer/RendererAPI.cpp
    src/Marble/Renderer/RendererAPI.h
    src/Marble/Renderer/Shader.cpp
    src/Marble/Renderer/Shader.h
    src/Marble/Renderer/Texture.cpp
    src/Marble/Renderer/Texture.h
    src/Marble/Renderer/VertexArray.cpp
    src/Marble/Renderer/VertexArray.h

    src/Platform/OpenGL/OpenGLBuffer.cpp
    src/Platform/OpenGL/OpenGLBuffer.h
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLContext.h
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.h
    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLShader.h
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/OpenGL/OpenGLTexture.h
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/OpenGL/OpenGLVertexArray.h
    
    src/Platform/Windows/WindowsInput.cpp
    src/Platform/Windows/WindowsInput.h
    src/Platform/Windows/WindowsWindow.cpp
    src/Platform/Windows/WindowsWindow.h
)

target_include_directories( Marble
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/spdlog/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/GLFW/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/Glad/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/stb_image
)

target_link_libraries(Marble Glad GLFW ImGui)

add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    GLFW_INCLUDE_NONE
)

if(UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    target_link_libraries(Marble GL X11 Xrandr Xi)
endif(UNIX)

if(WIN32)
    target_link_libraries(Marble opengl32.lib)
endif(WIN32)

install(TARGETS Marble DESTINATION lib)
install(FILES Marble.h DESTINATION include)