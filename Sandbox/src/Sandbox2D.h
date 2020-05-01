#pragma once

#include "Marble.h"
#include "Area2D.h"
#include "Marble/ECS/Entity/EntityManager.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"


#define MaxBalls 500

class Sandbox2D : public Marble::Layer {
public:
	Sandbox2D(); 
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Marble::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Marble::Event& e) override;

private:
	int numBalls = 100;

	struct Balls {
		std::array<glm::vec3, MaxBalls> Position;
		std::array<glm::vec3, MaxBalls> Velocity;
		std::array<float, MaxBalls> Mass;
		std::array<float, MaxBalls> Radius;
		std::array<float, MaxBalls> Rotation;
		std::array<glm::vec4, MaxBalls> Color;
		std::array<bool, MaxBalls> collided;

		Marble::Ref<Marble::Texture2D> m_Texture = Marble::Texture2D::Create("assets/textures/forward_circle.png");
	};

	Balls balls;

	ECS::EntityManager EntityManager;

	/*ECS::StackAllocator<100> stackAlloc;

	ECS::HeapAllocator<1024 * 1024 * 1024> heapAlloc;*/


	Marble::Scope<Area2D> area;

	Marble::OrthographicCameraController m_CameraController;

	int FramesPerSecond = 0;
	int FrameCounter = 0;
	float TimeSinceLastSecond = 0.0f;

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

};