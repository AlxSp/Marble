#include "mblpch.h"

#include "SimplexNoise.h"

/*based of https://github.com/SRombauts/SimplexNoise*/

namespace Marble {

	

	/*
	number of cornder for n dimension simplex:
		Corners = n + 1

	number of n dimension simplexes required to fill a n dimension hypercube:
		SimplexNumber = n!

	Skew factor formula for n dimension: 	
		SkewFactor = (sqrt(n + 1) - 1) / n

	Unskew factor formula for n dimension:
		UnskewFactor = SkewFactor / sqrt(n + 1)
	*/

	// Skewing/Unskewing factors for 2D
	static const float SkewFactor2D   = 0.366025403f;	// SkewFactor2D = (sqrt(2 + 1) - 1 ) / 2;
	static const float UnskewFactor2D = 0.211324865f;	// UnskewFactor2D = SkewFactor2D / sqrt(2 + 1)
	// Skewing/Unskewing factors for 3D
	static const float SkewFactor3D   = 0.333333333f;	// SkewFactor3D = (sqrt(3 + 1) - 1) / 3;
	static const float UnskewFactor3D = 0.166666666f;	// UnskewFactor3D = SkewFactor3D / sqrt(3 + 1)
	// Skewing/Unskewing factors for 4D
	static const float SkewFactor4D	  = 0.309016994f;	// SkewFactor4D = (sqrt(4 + 1) - 1) / 4;
	static const float UnskewFactor4D = 0.138196601f;	// UnskewFactor4D = SkewFactor4D / sqrt(4 + 1)

	static inline int32_t fastfloor(float fp) {
		return static_cast<int32_t>(fp + 32768.) - 32768;
	}

	static float grad(int32_t hash, float x) {
		const int32_t h = hash & 0x0F;	//Convert low 4 bits to hash code;
		float grad = 1.0f + (h & 7);	//Gradient value 1.0, 2.0, ..., 8.0
		if ((h & 8) != 0) grad = -grad;
		return (grad * x);
	}

	static float grad(int32_t hash, float x, float y) {
		const int32_t h = hash & 0x3F; //convert low 3 bits of hash code into 8 simple gradient directions
		const float u = h < 4 ? x : y; 
		const float v = h < 4 ? y : x;
		return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (x,y).
	}

	static float grad(int32_t hash, float x, float y, float z) {
		const int32_t h = hash & 15; // Convert low 4 bits of hash code into 12 simple
		float u = h < 8 ? x : y; // gradient directions, and compute dot product.
		float v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
		return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
	}

	static const uint8_t perm[256] = {
	151, 160, 137, 91, 90, 15,
	131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

	static inline uint8_t hash(int32_t i) {
		return perm[static_cast<uint8_t>(i)];
	}

	float SimplexNoise::Noise(float x)
	{
		float n0, n1; // Noise contributions from the two closest points

		// Point coordinates indexes (nearest integers)
		int32_t i0 = fastfloor(x);
		int32_t i1 = i0 + 1;
		// Calculate the distances between x and the points (between 0 and 1)
		float x0 = x - i0; 
		float x1 = x0 - 1.0f; // i1 - x ?

		//Calculate the contribution from the first corner
		float t0 = 1.0f - x0 * x0;
		t0 *= t0;
		n0 = t0 * t0 * grad(hash(i0), x0);

		//Calculate the contribution from the 


		return 0.0f;
	}

	float SimplexNoise::Noise(float x, float y)
	{
		float n0, n1, n2; //Noise contributions from the three corners 

		// Skew the input space into a signed distnace gird to determine which simplex cell the coordinates are in
		const float skewOffset = (x + y) * SkewFactor2D; // Skew factor for 2D
		//Get the hypercube's origin coodinates (corner closest to the coordinate system's origin)
		const int32_t i = fastfloor(x + skewOffset);	//Get x-axis index of hypercube 
		const int32_t j = fastfloor(y + skewOffset);	//Get y-axis index of hypercube

		// Unskew the hypercube's origin back to (x, y) space
		const float t = static_cast<int32_t>(i + j)* UnskewFactor2D;
		const float X0 = i - t;
		const float Y0 = j - t;
		// Calculate distances between the hypercube's origin and x,y
		const float x0 = x - X0;  
		const float y0 = y - Y0;
		// For the 2D case, the simplex shape is an equilateral triangle
		// Determine which simplex we are in
		int32_t i1, j1; // Offsets for second (middle) corner of simplex in (i, j) coordinates
		if (x0 > y0) {	// lower triangle of simplex, XY order: (0,0)->(1,0)->(1,1)
			i1 = 1;
			j1 = 0;
		} else {		// upper triangle of simplex, XY order: (0,0)->(0,1)->(1,1)
			i1 = 0;
			j1 = 1;
		}
		// a step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		const float x1 = x0 - i1 + UnskewFactor2D; // Offsets for middle corner in (x,y) unskewed coords
		const float y1 = y0 - j1 + UnskewFactor2D;
		const float x2 = x0 - 1.0 + 2.0 * UnskewFactor2D;
		const float y2 = y0 - 1.0 + 2.0 * UnskewFactor2D;

		// Calculate the hashed gradient indices of the three simplex corners
		const int gi0 = hash(i + hash(j));
		const int gi1 = hash(i + i1 + hash(j + j1));
		const int gi2 = hash(i + 1 + hash(j + 1));

		// Calculate the contribution from the three corners
		// Calculate the contribution from the first corner
		float t0 = 0.5f - x0 * x0 - y0 * y0;
		if (t0 < 0.0f) {
			n0 = 0.0f;
		}
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad(gi0, x0, y0);
		}

		// Calculate the contribution from the second corner
		float t1 = 0.5f - x1 * x1 - y1 * y1;
		if (t1 < 0.0f) {
			n1 = 0.0f;
		}
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad(gi1, x1, y1);
		}

		// Calculate the contribution from the third corner
		float t2 = 0.5f - x2 * x2 - y2 * y2;
		if (t2 < 0.0f) {
			n2 = 0.0f;
		}
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad(gi2, x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		return 45.23065f * (n0 + n1 + n2);
	}

	float SimplexNoise::Noise(float x, float y, float z)
	{
		float n0, n1, n2, n3; // Noise contributions from the four corners

		// Skew the input space into a signed distnace gird to determine which simplex cell the coordinates are in
		const float skewOffset = (x + y + z) * SkewFactor3D;
		// Get the hypercube's origin coodinates (corner closest to the coordinate system's origin)
		const int32_t i = fastfloor(x + skewOffset);	//Get x-axis index of hypercube's origin 
		const int32_t j = fastfloor(y + skewOffset);	//Get y-axis index of hypercube's origin
		const int32_t k = fastfloor(z + skewOffset);	//Get z-axis index of hypercube's origin

		// Unskew the hypercube's origin back to (x, y, z) space (representing the 1st corner of the simplex cell)
		const float t = static_cast<int32_t>(i + j + k) * UnskewFactor2D;
		const float X0 = i - t;
		const float Y0 = j - t;
		const float Z0 = k - t;
		// Calculate distances between the hypercube's origin and x, y, z
		const float x0 = x - X0;
		const float y0 = y - Y0;
		const float z0 = y - Z0; //dz0?
		// For the 2D case, the simplex shape is a slightly irregular tetrahedron
		// Determine which simplex we are in
		int32_t i1, j1, k1;	// 2nd corner of skewed simplex cell
		int32_t i2, j2, k2; // 3rd corner of skewed simplex cell
		if (x0 >= y0) {
			if (y0 >= z0) {
				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // X Y Z order
			}
			else if (x0 >= z0) {
				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; // X Z Y order
			}
			else {
				i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; // Z X Y order
			}
		}
		else { // x0<y0
			if (y0 < z0) {
				i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; // Z Y X order
			}
			else if (x0 < z0) {
				i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; // Y Z X order
			}
			else {
				i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // Y X Z order
			}
		}

		// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
		// A step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
		// A step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
		// Offsets for 2nd corner in (x,y,z) unskewed coords
		const float x1 = x0 - i1 + UnskewFactor3D; 
		const float y1 = y0 - j1 + UnskewFactor3D;
		const float z1 = z0 - k1 + UnskewFactor3D;
		// Offsets for 3rd corner in (x,y,z) unskewed coords
		const float x2 = x0 - 1.0 + 2.0 * UnskewFactor3D;
		const float y2 = y0 - 1.0 + 2.0 * UnskewFactor3D;
		const float z2 = z0 - 1.0 + 2.0 * UnskewFactor3D;
		// Offsets for last 4th corner in (x,y,z) unskewed coords
		float x3 = x0 - 1.0f + 3.0f * UnskewFactor3D;
		float y3 = y0 - 1.0f + 3.0f * UnskewFactor3D;
		float z3 = z0 - 1.0f + 3.0f * UnskewFactor3D;

		// Work out the hashed gradient indices of the four simplex corners
		int gi0 = hash(i + hash(j + hash(k)));
		int gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
		int gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
		int gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

		// Calculate the contribution from the four corners
		float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
		if (t0 < 0) {
			n0 = 0.0;
		}
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad(gi0, x0, y0, z0);
		}

		float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
		if (t1 < 0) {
			n1 = 0.0;
		}
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad(gi1, x1, y1, z1);
		}

		float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
		if (t2 < 0) {
			n2 = 0.0;
		}
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad(gi2, x2, y2, z2);
		}

		float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
		if (t3 < 0) {
			n3 = 0.0;
		}
		else {
			t3 *= t3;
			n3 = t3 * t3 * grad(gi3, x3, y3, z3);
		}
		// Add contributions from each corner to get the final noise value.
		// The result is scaled to stay just inside [-1,1]
		return 32.0f * (n0 + n1 + n2 + n3);
	}

	float SimplexNoise::Noise(float x, float y, float z, float w)
	{
		float n0, n1, n2, n3, n4; // Noise contributions from the five corners

		return 0;
	}

	float SimplexNoise::extrapolate(float x, float y, const size_t octaves, float frequency, float amplitude, const float lacunarity, const float persistence)
	{
		float numerator = 0.0f;
		float denominator = 0.0f;

		for (size_t i = 0; i < octaves; i++) {
			numerator += (amplitude * Noise(x * frequency, y * frequency));
			denominator += amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		return numerator / denominator;
	}

	float SimplexNoise::extrapolate(float x, float y, size_t octaves)
	{
		return extrapolate(x, y, octaves, m_Frequency, m_Amplitude, m_Lacunarity, m_Persistence);

		/*float numerator = 0.0f;
		float denominator  = 0.0f;

		float frequency = m_Frequency;
		float amplitude = m_Amplitude;

		for (size_t i = 0; i < octaves; i++) {
			numerator += (amplitude * Noise(x * frequency, y * frequency));
			denominator += amplitude;

			frequency *= m_Lacunarity;
			amplitude *= m_Persistence;
		}

		return numerator / denominator;*/
	}

}