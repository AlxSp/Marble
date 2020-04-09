#include "ObjectContainerManager2D.h"

ObjectContainerManager2D::ObjectContainerManager2D(const glm::vec2& playerPosition, const glm::vec2& containerSize) 
	: PlayerPostion(playerPosition), ContainerSize(containerSize)
{
	ContainerMap[InitiationOrigin] = new ColorContainer2D(InitiationOrigin, ContainerSize);
	MaxViewDistance = ContainerSize.x * 4;

	//ContainerMap[InitiationOrigin]->FillPixels(ContainerMap[InitiationOrigin]->GetPostion());
}

ObjectContainerManager2D::~ObjectContainerManager2D()
{
	for (std::pair<glm::vec2, ColorContainer2D*> element : ContainerMap)
	{
		delete element.second;
	}
	ContainerMap.clear();
}

void ObjectContainerManager2D::SetPlayerStatus(Nucleus::OrthographicCameraController& camController, const float& zoomDifference)
{
	PreviousPlayerPosition = PlayerPostion;  
	PlayerPostion = camController.GetPosition();
	PlayerDirection = PlayerPostion - PreviousPlayerPosition;

	PlayerView = glm::vec2{camController.GetAspectRatio() * camController.GetZoomLevel() * zoomDifference };
	ContainersInViewDistance = glm::round(PlayerView / ContainerSize);
	ContainersInViewDistance.x = glm::max(ContainersInViewDistance.x, 1);
	ContainersInViewDistance.y = glm::max(ContainersInViewDistance.y, 1);

}


void ObjectContainerManager2D::OnUpdate()
{
	float distance = 0.0f;
	float colorValue = 1.0f;

	std::unordered_map<glm::vec2, ColorContainer2D*>::iterator containerIter = ContainerMap.begin();

	while (containerIter != ContainerMap.end()) {
		distance = glm::distance(containerIter->first, PlayerPostion);
		if (distance > MaxViewDistance) {
			delete containerIter->second;
			containerIter = ContainerMap.erase(containerIter);
		}
		else {
			/*colorValue = 1.0f - distance / (ContainerSize.x * 5);
			glm::vec4 color = { colorValue, colorValue, colorValue, 1.0f };
			containerIter->second->SetColor(color);*/
			containerIter++;
		}
	}

}

void ObjectContainerManager2D::OnRender() {
	/*for (int i = 0; i < Detail * Detail; i++) {
		Containers[i]->OnRender();
	}*/
	glm::vec3 ViewOrigin = { PlayerPostion.x - PlayerView.x, PlayerPostion.y - PlayerView.y, 0.1f };
	Nucleus::BatchRenderer2D::DrawQuad(PlayerPostion, PlayerView * 2.0f, { 1.0f, .0666f, .0666f, .4f });
	// Iterate over an unordered_map using range based for loop
	for (std::pair<glm::vec2, ColorContainer2D* > element : ContainerMap)
	{
		element.second->OnRender();
		//std::cout << element.first << " :: " << element.second << std::endl;
	}
}

void ObjectContainerManager2D::GenerateContainers()
{
	glm::vec2 CurrentContainerPosition = (glm::abs(PlayerPostion) - glm::abs(InitiationOrigin)) / ContainerSize;

	CurrentContainerPosition.x *= PlayerPostion.x < 0 ? -1 : 1;
	CurrentContainerPosition.y *= PlayerPostion.y < 0 ? -1 : 1;

	CurrentContainerPosition = glm::round(CurrentContainerPosition) * ContainerSize + InitiationOrigin;

	for (int yOffset = -ContainersInViewDistance.y; yOffset <= ContainersInViewDistance.y; yOffset++) {
		for (int xOffset = -ContainersInViewDistance.x; xOffset <= ContainersInViewDistance.x; xOffset++) {
			glm::vec2 newContainerPosition = CurrentContainerPosition + glm::vec2{ xOffset , yOffset }  * ContainerSize;
			if (ContainerMap.find(newContainerPosition) == ContainerMap.end()) {
				ContainerMap[newContainerPosition] = new ColorContainer2D(newContainerPosition, ContainerSize);
				//ContainerMap[newContainerPosition]->FillPixels(newContainerPosition);
			}
		}
	}
	std::cout << "Container Map Size: " << ContainerMap.size() << std::endl;
}
