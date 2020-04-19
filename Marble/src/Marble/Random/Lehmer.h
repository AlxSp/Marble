#pragma once
#include "mblpch.h"

#include <cstdint> 

namespace Marble {

	class Lehmer32 {
	public:
		Lehmer32(uint32_t seed = 0) : lehmer_state(seed) {}

		void seed(uint32_t seed) {
			lehmer_state = seed;
		}

		uint32_t randInt() {
			lehmer_state += 0xe120fc15;
			uint64_t tmp;
			tmp = (uint64_t)lehmer_state * 0x4a39b70d;
			uint32_t m1 = (tmp >> 32) ^ tmp;
			tmp = (uint64_t)m1 * 0x12fad5c9;
			uint32_t m2 = (tmp >> 32) ^ tmp;
			return m2;
		}

		uint32_t randInt(int min, int max) {
			return (randInt() % (max - min)) + min;
		}

		double randDouble(double min, double max) {
			return ((double)randInt() / (double)(0x7FFFFFFF) * (max - min) + min);
		}

		float randFloat(float min, float max) {
			return ((float)randInt() / (float)(0x7FFFFFFF) * (max - min) + min);
		}

	private:
		uint32_t lehmer_state;
	};

}