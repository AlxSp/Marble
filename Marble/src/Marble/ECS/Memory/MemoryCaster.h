#pragma once
#include "Marble/ECS/Memory/MemoryAllocator.h"

namespace ECS {

	template<typename type>
	struct Array {
		type* ptr;
		size_t size;
	};

	template<typename type>
	type* GetAs(void* ptr) {
		return static_cast<type*>ptr;
	}

	template<typename type>
	type GetValue(void* ptr) {
		return *static_cast<type*>(ptr);
	}
}

