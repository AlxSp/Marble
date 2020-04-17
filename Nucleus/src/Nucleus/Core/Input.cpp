#include "ncpch.h"
#include "Nucleus/Core/Input.h"

#ifdef NC_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#elif defined(NC_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsInput.h"
#endif

namespace Nucleus {
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
        #ifdef NC_PLATFORM_WINDOWS
            return CreateScope<WindowsInput>();
        #elif defined(NC_PLATFORM_LINUX)
            return CreateScope<WindowsInput>();
        #else
            NC_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
        #endif 
    }
}