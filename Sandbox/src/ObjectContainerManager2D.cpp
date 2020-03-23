#include "ObjectContainerManager2D.h"

ObjectContainerManager2D::ObjectContainerManager2D(const glm::vec2& playerPosition, const glm::vec2& containerSize) 
	: PlayerPostion(playerPosition), ContainerSize(containerSize)
{
	ContainerHook = PlayerPostion - ContainerSize * 0.5f;
	Containers.push_back(new NoiseContainer2D(ContainerHook, ContainerSize, 128));
	Containers[0]->FillPixels(Containers[0]->GetPostion());
}

ObjectContainerManager2D::~ObjectContainerManager2D()
{
	for (int i = 0; i < Containers.size(); i++) {
		delete Containers[i];
	}
}


void ObjectContainerManager2D::OnRender() {
	/*for (int i = 0; i < Detail * Detail; i++) {
		Containers[i]->OnRender();
	}*/
	Containers[0]->OnRender();
	//for (int i = 0; i < NumberOfContainers)
}

void ObjectContainerManager2D::UpdateContainers()
{
	const glm::vec2 ContainerCheck1 = ContainerHook;
	const glm::vec2 ContainerCheck2 = ContainerHook + ContainerSize;

	bool toUpdate = false;
	glm::vec2 nextHook = { 0.0f, 0.0f };

	if (ContainerCheck1.x > PlayerPostion.x) {
		std::cout << "Out of Bounds! Count " << counter++ << std::endl;
		nextHook.x = -1.0f;
		toUpdate = true;
	}
	if (ContainerCheck1.y > PlayerPostion.y) {
		std::cout << "Out of Bounds! Count " << counter++ << std::endl;
		nextHook.y = -1.0f;
		toUpdate = true;
	}
	if (ContainerCheck2.x < PlayerPostion.x) {
		std::cout << "Out of Bounds! Count " << counter++ << std::endl;
		nextHook.x = 1.0f;
		toUpdate = true;
	}
	if (ContainerCheck2.y < PlayerPostion.y) {
		std::cout << "Out of Bounds! Count " << counter++ << std::endl;
		nextHook.y = 1.0f;
		toUpdate = true;
	}

	if (toUpdate) {
		delete Containers[0];
		ContainerHook = ContainerHook + nextHook * ContainerSize;
		Containers[0] = new NoiseContainer2D(ContainerHook, ContainerSize, 128);
		Containers[0]->FillPixels(Containers[0]->GetPostion());
	}
}
