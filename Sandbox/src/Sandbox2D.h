#pragma once

#include "Marble.h"
#include "Ball.h"
#include "Area2D.h"

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
	//Ball* testBall;
	int numBalls = 100;

	// struct Balls {
	// 	std::array<glm::vec3, MaxBalls> Position;
	// 	std::array<glm::vec2, MaxBalls> Velocity;
	// 	std::array<float, MaxBalls> Mass;
	// 	std::array<float, MaxBalls> Radius;
	// 	std::array<float, MaxBalls> Rotation;

	// 	Marble::Ref<Marble::Texture2D> m_Texture = Marble::Texture2D::Create("assets/textures/forward_circle.png");
	// };
	//std::array<Ball, 500> cellArray;
	std::vector<Ball> cellVector;
	float areaScale = 5.0f;

	Marble::Scope<Area2D> area;// = Area2D(16.0f * areaScale, 9.0f * areaScale);

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