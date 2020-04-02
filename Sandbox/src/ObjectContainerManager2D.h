#pragma once
#include "Nucleus.h"
#include "Container2D.h"

#include <glm/gtx/hash.hpp>
#include <map>
//Procedural
//Procedural Grid based object Container. Will have to be more flexible later 
class ObjectContainerManager2D {
public:
	ObjectContainerManager2D(const glm::vec2& playerPosition, const glm::vec2& containerSize);
	~ObjectContainerManager2D();

	void SetPlayerStatus(Nucleus::OrthographicCameraController& controller, const float& zoomDifference = 1.0f);
	void SetPlayerPosition(const glm::vec2& position) { PreviousPlayerPosition = PlayerPostion;  PlayerPostion = position; PlayerDirection = PlayerPostion - PreviousPlayerPosition;}
	void SetPlayerView(const float& aspectRatio, const float& zoomLevel) { PlayerView = { aspectRatio * zoomLevel, aspectRatio * zoomLevel }; ContainersInViewDistance = glm::round(PlayerView / ContainerSize); }

	void OnUpdate();
	void OnRender();

	void GenerateContainers();
	
private:
	

private:
	glm::vec2 PlayerPostion;
	glm::vec2 PreviousPlayerPosition = { 0.0f, 0.0f };
	glm::vec2 PlayerDirection = { 0.0f, 0.0f }; //Normlized Vector
	glm::vec2 PlayerView = { 0.0f, 0.0f };


	float MaxViewDistance = 0.0f;

	glm::vec2 InitiationOrigin = { 0.0f, 0.0f };
	glm::vec2 ContainerSize;
	glm::ivec2 ContainersInViewDistance = { 0, 0 };

	//std::vector<NoiseContainer2D*> Containers;
	std::unordered_map<glm::vec2, ColorContainer2D*> ContainerMap;
	//tmp
	int counter = 0;

};