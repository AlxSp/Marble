#include "mblpch.h"
#include "Marble/Core/Window.h"

#ifdef MBL_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#elif defined(MBL_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Marble {
    Scope<Window> Window::Create(const WindowProps& props)
    {
        #ifdef MBL_PLATFORM_WINDOWS
            return CreateScope<WindowsWindow>(props);
        #elif defined(MBL_PLATFORM_LINUX)
            return CreateScope<WindowsWindow>(props);
        #else
            MBL_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
        #endif 
    }
}