#include "ObjectContainerManager2D.h"

#include <thread>

#include "imgui/imgui.h"


ObjectContainerManager2D::ObjectContainerManager2D(const Marble::Ref<Player> player, const glm::vec2& containerSize)
	:  m_Player(player), ContainerSize(containerSize)
{
	ContainerMap[InitiationOrigin] = new MapContainer(InitiationOrigin, ContainerSize);
	MaxViewDistance = ContainerSize.x * 4;
}

ObjectContainerManager2D::~ObjectContainerManager2D()
{
	for (std::pair<glm::vec2, MapContainer*> element : ContainerMap)
	{
		delete element.second;
	}
	ContainerMap.clear();
}

void ObjectContainerManager2D::OnUpdate()
{
	GenerateContainers();
	float distance = 0.0f;
	float zoomLevel = m_Player->GetZoomLevel();
	std::unordered_map<glm::vec2, MapContainer*>::iterator containerIter = ContainerMap.begin();

	deletionCount = 0;
	while (containerIter != ContainerMap.end()) {
		distance = glm::distance(containerIter->first, m_Player->GetCameraPosition());
		if (distance > MaxViewDistance ) {
			delete containerIter->second;
			containerIter = ContainerMap.erase(containerIter);
			deletionCount++;
		}
		else {
			containerIter->second->OnUpdate(zoomLevel);
			containerIter++;
		}
	}
}

void ObjectContainerManager2D::OnRender() {
	for (std::pair<glm::vec2, MapContainer* > element : ContainerMap)
	{
		element.second->OnRender();
	}
	Marble::Renderer2D::DrawQuad(glm::vec3(m_Player->GetCameraPosition(), 0.1f), m_Player->GetCameraView() * ZoomDifference, { 1.0f, .0666f, .0666f, .3f });
}

void ObjectContainerManager2D::OnImGuiRender()
{
	ImGui::Begin("Object Container Manager");
	ImGui::Text("Containers: %i", ContainerMap.size());
	ImGui::Text("Deletions: %i", deletionCount);
	ImGui::Text("Container Size: %f", ContainerMap.size());
	ImGui::Text("Containers in View (x): %i", ContainersInViewDistance.x);
	ImGui::Text("Containers in View (y): %i", ContainersInViewDistance.y);
	ImGui::End();
}

void ObjectContainerManager2D::GenerateContainers()
{
	ContainersInViewDistance = glm::round(m_Player->GetCameraView() * ZoomDifference * 0.5f / ContainerSize);
	ContainersInViewDistance.x = glm::max(ContainersInViewDistance.x, 1);
	ContainersInViewDistance.y = glm::max(ContainersInViewDistance.y, 1);

	glm::vec2 CurrentContainerPosition = glm::round((m_Player->GetCameraPosition() - InitiationOrigin) / ContainerSize) * ContainerSize + InitiationOrigin;

	for (int yOffset = -ContainersInViewDistance.y; yOffset <= ContainersInViewDistance.y; yOffset++) {
		for (int xOffset = -ContainersInViewDistance.x; xOffset <= ContainersInViewDistance.x; xOffset++) {
			glm::vec2 newContainerPosition = CurrentContainerPosition + glm::vec2{ xOffset , yOffset }  * ContainerSize;
			if (ContainerMap.find(newContainerPosition) == ContainerMap.end()) {
				ContainerMap[newContainerPosition] = new MapContainer(newContainerPosition, ContainerSize);
			}
		}
	}
}
