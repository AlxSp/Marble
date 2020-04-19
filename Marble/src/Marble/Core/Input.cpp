#include "mblpch.h"
#include "Marble/Core/Input.h"

#ifdef MBL_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#elif defined(MBL_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsInput.h"
#endif

namespace Marble {
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
        #ifdef MBL_PLATFORM_WINDOWS
            return CreateScope<WindowsInput>();
        #elif defined(MBL_PLATFORM_LINUX)
            return CreateScope<WindowsInput>();
        #else
            MBL_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
        #endif 
    }
}