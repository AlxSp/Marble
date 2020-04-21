#pragma once
#include "Marble.h"
#include "Container2D.h"
#include "Player.h"

#include <glm/gtx/hash.hpp>
#include <map>

typedef ColorContainer2D MapContainer;
//Procedural
//Procedural Grid based object Container. Will have to be more flexible later 
class ObjectContainerManager2D {
public:
	ObjectContainerManager2D(const Marble::Ref<Player> player, const glm::vec2& containerSize);
	~ObjectContainerManager2D();

	void SetPlayer(Marble::Ref<Player> player) { m_Player = player; }
	void SetZoomDifference(const float& diff) { ZoomDifference = diff; }

	void OnUpdate();
	void OnRender();

	void OnImGuiRender();

	void GenerateContainers();
	
private:
	int deletionCount = 0;

private:
	Marble::Ref<Player> m_Player = nullptr;


	float MaxViewDistance = 0.0f;
	float ZoomDifference = 1.0f;


	glm::vec2 InitiationOrigin = { 0.0f, 0.0f };
	glm::vec2 lastContainerPosition = { 0.0f, 0.0f };
	glm::vec2 ContainerSize;
	glm::ivec2 ContainersInViewDistance = { 0, 0 };

	//std::vector<NoiseContainer2D*> Containers;
	
	std::unordered_map<glm::vec2, MapContainer*> ContainerMap;
	//tmp
	int counter = 0;

};