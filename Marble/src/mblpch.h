#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>
#include <utility>
#include <filesystem>
#include <csignal>

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Marble/Core/Log.h"
#include "Marble/Debug/Instrumentor.h"

#include "Marble/Core/KeyCodes.h"

#if defined MBL_PLATFORM_WINDOWS
	#include <Windows.h>
#elif defined MBL_PLATFORM_LINUX
	#include <unistd.h>
#endif //MBL_PLATFORM_WINDOWS