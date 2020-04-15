#pragma once
#include "Nucleus.h"

struct TerrainType
{
	glm::vec2 heightRange;
	glm::ivec4 color;
};

class TerrainColorMap {
public:
	TerrainColorMap();

	glm::ivec4 getColor(float& height);

private:
	std::vector<TerrainType> Regions;
};

static TerrainColorMap terrainColorMap;