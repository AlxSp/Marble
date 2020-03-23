#pragma once
#include "Nucleus.h"
#include "Container2D.h"

//Procedural
//Procedural Grid based object Container. Will have to be more flexible later 
class ObjectContainerManager2D {
public:
	ObjectContainerManager2D(const glm::vec2& playerPosition, const glm::vec2& containerSize);
	~ObjectContainerManager2D();

	void SetPlayerPosition(const glm::vec2& position) { PlayerPostion = position; UpdateContainers(); }
	void SetPlayerView(const float& aspectRatio, const float& zoomLevel) { PlayerView = { aspectRatio * zoomLevel, zoomLevel }; }

	void OnRender();

private:
	void UpdateContainers();

private:
	glm::vec2 PlayerPostion;
	glm::vec2 PlayerView = { 0.0, 0.0 };

	glm::vec2 ContainerHook = { 0.0, 0.0f };
	glm::vec2 ContainerSize;

	std::vector<NoiseContainer2D*> Containers;
	//tmp
	int counter = 0;

};