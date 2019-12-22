#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>
#include <utility>

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Nucleus/Core/Log.h"
#include "Nucleus/Debug/Instrumentor.h"

#include "Nucleus/Core/KeyCodes.h"

#ifdef NC_PLATFORM_WINDOWS
	#include <Windows.h>
#endif //NC_PLATFORM_WINDOWS