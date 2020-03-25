#include "ObjectContainerManager2D.h"

ObjectContainerManager2D::ObjectContainerManager2D(const glm::vec2& playerPosition, const glm::vec2& containerSize) 
	: PlayerPostion(playerPosition), ContainerSize(containerSize)
{
	//ContainerHook = PlayerPostion - ContainerSize * 0.5f;
	//Containers.push_back(new NoiseContainer2D(ContainerHook, ContainerSize, 128));

	ContainerMap[InitiationOrigin] = new NoiseContainer2D(InitiationOrigin, ContainerSize, 128);
	ContainerMap[InitiationOrigin]->FillPixels(ContainerMap[InitiationOrigin]->GetPostion());
	//Containers[0]->FillPixels(Containers[0]->GetPostion());
}

ObjectContainerManager2D::~ObjectContainerManager2D()
{
	for (std::pair<glm::vec2, NoiseContainer2D*> element : ContainerMap)
	{
		delete element.second;
	}
	ContainerMap.clear();
}


void ObjectContainerManager2D::OnRender() {
	/*for (int i = 0; i < Detail * Detail; i++) {
		Containers[i]->OnRender();
	}*/
	glm::vec3 ViewOrigin = { PlayerPostion.x - PlayerView.x, PlayerPostion.y - PlayerView.y, 0.1f };
	//Nucleus::BatchRenderer2D::DrawQuad(ViewOrigin, PlayerView * 2.0f, { 1.0f, .0666f, .0666f, .4f });
	// Iterate over an unordered_map using range based for loop
	for (std::pair<glm::vec2, NoiseContainer2D* > element : ContainerMap)
	{
		element.second->OnRender();
		//std::cout << element.first << " :: " << element.second << std::endl;
	}
	//Containers[0]->OnRender();
	//for (int i = 0; i < NumberOfContainers)
	
}

void ObjectContainerManager2D::UpdateContainers()
{
	glm::vec2 CurrentContainerPosition = (glm::abs(PlayerPostion) - glm::abs(InitiationOrigin)) / ContainerSize;

	CurrentContainerPosition.x *= PlayerPostion.x < 0 ? -1 : 1;
	CurrentContainerPosition.y *= PlayerPostion.y < 0 ? -1 : 1;

	CurrentContainerPosition = glm::round(CurrentContainerPosition) * ContainerSize + InitiationOrigin;

	for (int yOffset = -ContainersInViewDistance.y; yOffset <= ContainersInViewDistance.y; yOffset++) {
		for (int xOffset = -ContainersInViewDistance.x; xOffset <= ContainersInViewDistance.x; xOffset++) {
			glm::vec2 newContainerPosition = CurrentContainerPosition + glm::vec2{ xOffset , yOffset }  *ContainerSize;
			if (ContainerMap.find(newContainerPosition) == ContainerMap.end()) {
				//chunksGenerated++;

				std::cout << "Container Created at: " << newContainerPosition.x << " " << newContainerPosition.y << std::endl;
				std::cout << "Player at: "<< PlayerPostion.x << " " << PlayerPostion.y << std::endl;
				ContainerMap[newContainerPosition] = new NoiseContainer2D(newContainerPosition, ContainerSize, 128);
				ContainerMap[newContainerPosition]->FillPixels(ContainerMap[newContainerPosition]->GetPostion());
			}
		}
	}
	std::cout << "Container Map Size: " << ContainerMap.size() << std::endl;
	//const glm::vec2 ContainerCheck1 = ContainerHook;
	//const glm::vec2 ContainerCheck2 = ContainerHook + ContainerSize;

	//bool toUpdate = false;
	//glm::vec2 nextHook = { 0, 0 };

	////std::vector<glm::vec2> nextHooks;

	//if (ContainerCheck1.x > PlayerPostion.x) {
	//	//nextHooks.push_back({ -1.0f, 0.0f });
	//	nextHook.x = -1;
	//	toUpdate = true;
	//}
	//else if (ContainerCheck2.x < PlayerPostion.x) {
	//	//nextHooks.push_back({ 1.0f, 0.0f });
	//	nextHook.x = 1;
	//	toUpdate = true;
	//}

	//if (ContainerCheck1.y > PlayerPostion.y) {
	//	//nextHooks.push_back({ 0.0f, -1.0f });
	//	nextHook.y = -1;
	//	toUpdate = true;
	//}
	//else if (ContainerCheck2.y < PlayerPostion.y) {
	//	//nextHooks.push_back({ 0.0f, 1.0f });
	//	nextHook.y = 1;
	//	toUpdate = true;
	//}

	//if (toUpdate) {
	//	std::cout << "Out of Bounds! Count " << counter++ << std::endl;
	//	//glm::vec2 nextHook = { 0.0f, 0.0f };
	//	int chunksGenerated = 0;
	//	std::cout << "" << std::endl;
	//	for (int i = 0; i < 2; i++) {
	//		for (int j = 0; i < 2; i++) {
	//			//nextHook = ContainerHook + (glm::vec2{i, j} + nextHook) * ContainerSize;
	//			std::cout << "Creating Container at : " << nextHook.x << " " << nextHook.y << std::endl;
	//			if (ContainerMap.find(nextHook) == ContainerMap.end()) {
	//				chunksGenerated++;
	//				ContainerMap[nextHook] = new NoiseContainer2D(nextHook, ContainerSize, 128);
	//				ContainerMap[nextHook]->FillPixels(ContainerMap[nextHook]->GetPostion());
	//			}
	//		}
	//	}
	//	
	//	std::cout << "Chunks Generated: " << chunksGenerated << std::endl;
	//	std::cout << "Container Map Size: " << ContainerMap.size() << std::endl;
	
}
