#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>
#include <utility>
#include <filesystem>

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Nucleus/Core/Log.h"
#include "Nucleus/Debug/Instrumentor.h"

#include "Nucleus/Core/KeyCodes.h"

#if defined NC_PLATFORM_WINDOWS
	#include <Windows.h>
#elif defined NC_PLATFORM_LINUX
	#include <unistd.h>
#endif //NC_PLATFORM_WINDOWS