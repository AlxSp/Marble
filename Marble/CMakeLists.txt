# Marble Engine

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(
    Marble
    VERSION 0.1
    LANGUAGES CXX
)

add_library( Marble STATIC 
    src/Marble.h        
)

#Precompiled headers
target_precompile_headers( Marble
    PRIVATE
        src/mblpch.cpp
        src/mblpch.h
)

#Core
target_sources( Marble
    PRIVATE
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
)

#Debugging
target_sources( Marble
    PRIVATE
        src/Marble/Debug/Instrumentor.h
)

#Events
target_sources( Marble
    PRIVATE
        src/Marble/Events/Event.h
        src/Marble/Events/ApplicationEvent.h
        src/Marble/Events/KeyEvent.h
        src/Marble/Events/MouseEvent.h
)

#ImGui implementation
target_sources( Marble
    PRIVATE
        src/Marble/ImGui/ImGuiBuild.cpp
        src/Marble/ImGui/ImGuiLayer.cpp
        src/Marble/ImGui/ImGuiLayer.h
)

#Noise and Random Generators
target_sources( Marble
    PRIVATE
        #Noise
        src/Marble/Noise/SimplexNoise.cpp
        src/Marble/Noise/SimplexNoise.h
        #RNG
        src/Marble/Random/Lehmer.h
)

#Renderer
target_sources( Marble
    PRIVATE
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
        src/Marble/Renderer/Renderer2DQuadShader.h
        src/Marble/Renderer/RendererAPI.cpp
        src/Marble/Renderer/RendererAPI.h
        src/Marble/Renderer/Shader.cpp
        src/Marble/Renderer/Shader.h
        src/Marble/Renderer/Texture.cpp
        src/Marble/Renderer/Texture.h
        src/Marble/Renderer/VertexArray.cpp
        src/Marble/Renderer/VertexArray.h
)

#OpenGL code
target_sources( Marble
    PRIVATE
        src/RendererAPI/OpenGL/OpenGLBuffer.cpp
        src/RendererAPI/OpenGL/OpenGLBuffer.h
        src/RendererAPI/OpenGL/OpenGLContext.cpp
        src/RendererAPI/OpenGL/OpenGLContext.h
        src/RendererAPI/OpenGL/OpenGLRendererAPI.cpp
        src/RendererAPI/OpenGL/OpenGLRendererAPI.h
        src/RendererAPI/OpenGL/OpenGLShader.cpp
        src/RendererAPI/OpenGL/OpenGLShader.h
        src/RendererAPI/OpenGL/OpenGLTexture.cpp
        src/RendererAPI/OpenGL/OpenGLTexture.h
        src/RendererAPI/OpenGL/OpenGLVertexArray.cpp
        src/RendererAPI/OpenGL/OpenGLVertexArray.h
)
#Windows Platform code
target_sources( Marble
    PRIVATE
        src/Platform/Windows/WindowsInput.cpp
        src/Platform/Windows/WindowsInput.h
        src/Platform/Windows/WindowsWindow.cpp
        src/Platform/Windows/WindowsWindow.h
)

### Third Party Libraries ###

####
# Add Glad as library
add_library( Glad STATIC
    thirdParty/Glad/include/KHR/khrplatform.h
    thirdParty/Glad/src/glad.c
)

target_include_directories( Glad
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/Glad/include
)
####

####
# Add stb_image as library
add_library( stb_image STATIC
    thirdParty/stb_image/stb_image.cpp
    thirdParty/stb_image/stb_image.h
)

target_include_directories( stb_image
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/stb_image
)
####

####
# Add imgui as library
add_library( imgui STATIC
    thirdParty/imgui/imconfig.h
    thirdParty/imgui/imgui.h
    thirdParty/imgui/imgui.cpp
    thirdParty/imgui/imgui_draw.cpp
    thirdParty/imgui/imgui_internal.h
    thirdParty/imgui/imgui_widgets.cpp
    thirdParty/imgui/imstb_rectpack.h
    thirdParty/imgui/imstb_textedit.h
    thirdParty/imgui/imstb_truetype.h
    thirdParty/imgui/imgui_demo.cpp
)

target_include_directories( imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/imgui
)
####

####
# Add glfw subdirectory
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# add_compile_definitions(
#     _CRT_SECURE_NO_WARNINGS
#     GLFW_INCLUDE_NONE
# )
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/GLFW)
####

####
# Add glm subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glm)
####

####
# Add spdlog as subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/spdlog)
####

####
# Add EnTT as subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/EnTT)
####

#Compile Marble with c++17 <
target_compile_features(Marble PUBLIC cxx_std_17)

target_include_directories( Marble
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries( Marble
    glfw 
    imgui
    Glad
    glm
    spdlog
    stb_image
    EnTT
)

#Set build type dependent definitions
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Marble PUBLIC MBL_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(Marble PUBLIC MBL_RELEASE)
endif()

if(UNIX)
    set_target_properties( Marble PROPERTIES POSITION_INDEPENDENT_CODE ON)
    #set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    target_link_libraries( Marble
        GL 
        X11 
        Xrandr 
        Xi
    )
elseif(WIN32)
    target_link_libraries( Marble 
        opengl32.lib
    )
endif()

# install(TARGETS Marble DESTINATION lib)
# install(FILES src/Marble.h DESTINATION include)