#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>
#include <random>

float randomNumberGen(float min, float max) {
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(min, max);

	return dist(e2);
}

auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= (r1 + r2) * (r1 + r2);
};

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
	std::cout << "Possible Threads: " << std::thread::hardware_concurrency() << std::endl;
	m_CameraController.SetZoomLevel(25.0f);
	//testBall = new Ball();
	area = Marble::CreateScope<Area2D>(Area2D({m_CameraController.GetAspectRatio() * 2 * m_CameraController.GetZoomLevel(), 1 * 2 * m_CameraController.GetZoomLevel()}));

	area->SetColorRGBA(0, 85, 130);
	//area->SetPosition(10.0f, 0.0f);
	//float spawnAreaWidth = area->Dimension.x * 0.5f - area->Dimension.x * 0.1f;
	//float spawnAreaHeight = area->Dimension.y * 0.5f - area->Dimension.y * 0.1f;
	for (int i = 0; i < numBalls; i++) {
		cellVector.push_back(Ball({ randomNumberGen(area->leftBorder, area->rightBorder), randomNumberGen(area->bottomBorder, area->topBorder), (i / (numBalls + 1.0f))}));
		
		cellVector[i].SetMass(randomNumberGen(1, 5));
		//cellVector[i].SetArea(&Area);
		cellVector[i].SetVelocity({ randomNumberGen(-3, 3), randomNumberGen(-3, 3),  }); //randomNumberGen(-1, 1)
	}
}

void Sandbox2D::OnAttach()
{
	MBL_PROFILE_FUNCTION();
}

void Sandbox2D::OnDetach()
{
	MBL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Marble::TimeStep ts)
{
	MBL_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);

	FrameCounter++;
	TimeSinceLastSecond += ts;
	if (TimeSinceLastSecond >= 1.0f){
		FramesPerSecond = FrameCounter;
		FrameCounter = 0;
		TimeSinceLastSecond = 0;
	}

	for (int i = 0; i < numBalls; i++) {
		cellVector[i].OnUpdate(ts);
		
		if (cellVector[i].Position.x >= area->rightBorder) cellVector[i].Position.x = area->leftBorder;
		if (cellVector[i].Position.x <	area->leftBorder) cellVector[i].Position.x = area->rightBorder;
		if (cellVector[i].Position.y >= area->topBorder) cellVector[i].Position.y = area->bottomBorder;
		if (cellVector[i].Position.y < area->bottomBorder) cellVector[i].Position.y = area->topBorder;
		
	}
	
	std::vector<std::pair<Ball*, Ball*>> vecCollidedPairs;

	for (int i = 0; i < numBalls; i++) {
		//cellVector[i].CheckForCollisions(cellVector, i);
		for (int j = i + 1; j < numBalls; j++) {
			if (i != j) {
				if (DoCirclesOverlap(cellVector[i].Position.x, cellVector[i].Position.y, cellVector[i].Radius, cellVector[j].Position.x, cellVector[j].Position.y, cellVector[j].Radius)) {

					vecCollidedPairs.push_back({&cellVector[i], &cellVector[j]});

					cellVector[i].collided = true;
					cellVector[j].collided = true;


					float distance = sqrtf(pow((cellVector[i].Position.x - cellVector[j].Position.x), 2.0f) + pow((cellVector[i].Position.y - cellVector[j].Position.y), 2.0f));

					float overlap = (distance - cellVector[i].Radius - cellVector[j].Radius) * 0.5f;

					cellVector[i].Position.x -= overlap * (cellVector[i].Position.x - cellVector[j].Position.x) / distance;
					cellVector[i].Position.y -= overlap * (cellVector[i].Position.y - cellVector[j].Position.y) / distance;

					cellVector[j].Position.x += overlap * (cellVector[i].Position.x - cellVector[j].Position.x) / distance;
					cellVector[j].Position.y += overlap * (cellVector[i].Position.y - cellVector[j].Position.y) / distance;
				}
			}
		}
	}

	for (auto collision : vecCollidedPairs) {
		Ball* cell1 = collision.first;
		Ball* cell2 = collision.second;
		
		float distance = sqrtf(pow((cell1->Position.x - cell2->Position.x), 2.0f) + pow((cell1->Position.y - cell2->Position.y), 2.0f));

		float nx = (cell2->Position.x - cell1->Position.x) / distance;
		float ny = (cell2->Position.y - cell1->Position.y) / distance;

		float tx = -ny;
		float ty = nx;

		float dpTan1 = cell1->Velocity.x * tx + cell1->Velocity.y * ty;
		float dpTan2 = cell2->Velocity.x * tx + cell2->Velocity.y * ty;

		float dpNorm1 = cell1->Velocity.x * nx + cell1->Velocity.y * ny;
		float dpNorm2 = cell2->Velocity.x * nx + cell2->Velocity.y * ny;

		float m1 = (dpNorm1 * (cell1->Mass - cell2->Mass) + 2.0f * cell2->Mass * dpNorm2) / (cell1->Mass + cell2->Mass);
		float m2 = (dpNorm2 * (cell2->Mass - cell1->Mass) + 2.0f * cell1->Mass * dpNorm1) / (cell1->Mass + cell2->Mass);

		cell1->Velocity.x = tx * dpTan1 + nx * m1;
		cell1->Velocity.y = ty * dpTan1 + ny * m1;
		
		cell2->Velocity.x = tx * dpTan2 + nx * m2;
		cell2->Velocity.y = ty * dpTan2 + ny * m2;
		
	}
	
	// Render
	{
		MBL_PROFILE_SCOPE("Render Prep");

		Marble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Marble::RenderCommand::Clear();
	}
	{
		MBL_PROFILE_SCOPE("Render Draw");
		Marble::Renderer2D::ResetStats();
		Marble::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//testBall->OnRender();
		area->OnRender();
		//Marble::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, Area2D, { 0/255.0f, 85/255.0f, 130/255.0f, 1.0f });
		for (int i = 0; i < numBalls; i++) {
			cellVector[i].OnRender();
		}
		//Marble::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 10.0f, 10.0f }, m_TextureShader);
		Marble::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	MBL_PROFILE_FUNCTION();

	ImGui::Begin("Renderer2D Stats");
	auto stats = Marble::Renderer2D::GetStats();
	ImGui::Text("FPS: %i", FramesPerSecond);
	ImGui::Text("Draw Calls: %i", stats.DrawCalls);
	ImGui::Text("Quad Count: %i", stats.QuadCount);
	ImGui::Text("Vertices: %i", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %i", stats.GetTotalIndexCount());
	ImGui::End();
	/*
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
	*/
}

void Sandbox2D::OnEvent(Marble::Event& e)
{
	m_CameraController.OnEvent(e);
}

