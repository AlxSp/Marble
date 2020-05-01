#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>

namespace ECS {
	using EntityID = uint64_t;
	using ComponentID = uint64_t;
	using Type = std::vector<ComponentID>;
}
