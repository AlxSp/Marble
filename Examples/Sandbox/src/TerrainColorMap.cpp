#include "TerrainColorMap.h"


TerrainColorMap::TerrainColorMap()
{
	Regions.push_back({
		{0.0f, 0.4f},
		{52, 164, 235, 255}
		});

	Regions.push_back({
		{0.4f, 0.7f},
		{124, 235, 52, 255}
		});

	Regions.push_back({
		{0.7f, 0.9f},
		{112, 106, 92, 255}
		});

	Regions.push_back({
		{0.9f, 1.0f},
		{255, 255, 255, 255}
		});

}

glm::ivec4 TerrainColorMap::getColor(float& height)
{
	for (std::vector<TerrainType>::iterator it = Regions.begin(); it != Regions.end(); ++it) {
		if ((height - it->heightRange.x) <= (it->heightRange.y - it->heightRange.x)) {
			return it->color;
		}
	}
	return {0,0,0,255};
}
