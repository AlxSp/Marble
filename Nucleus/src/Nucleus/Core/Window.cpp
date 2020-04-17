#include "ncpch.h"
#include "Nucleus/Core/Window.h"

#ifdef NC_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#elif defined(NC_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Nucleus {
    Scope<Window> Window::Create(const WindowProps& props)
    {
        #ifdef NC_PLATFORM_WINDOWS
            return CreateScope<WindowsWindow>(props);
        #elif defined(NC_PLATFORM_LINUX)
            return CreateScope<WindowsWindow>(props);
        #else
            NC_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
        #endif 
    }
}