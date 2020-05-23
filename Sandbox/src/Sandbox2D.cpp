#include "Sandbox2D.h"
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Marble/ECS/Entity/EntityDefinitions.h"
//#include "Marble/ECS/Memory/MemoryCaster.h"

#include <chrono>
#include <random>


struct Position {
	float x;
	float y;
	float z;
};

struct Velocity {
	float x;
	float y;
	float z;
};

struct Mass {
	float x;
};

struct Radius {
	float x;
};

struct Rotation {
	float x;
};

struct Color {
	float r;
	float g;
	float b;
	float a;
};

struct Collided {
	bool collided;
};


//void UpdatePosition(ECS::EntityManager& EntityManager, float& dt) {
//	EntityManager.System<Position, Velocity>().each(
//		[&dt](auto& pos, auto& vel) {
//			pos->x += vel->x * dt;
//			pos->y += vel->y * dt;
//		}
//	);
//}


float randomNumberGen(float min, float max) {
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(min, max);

	return dist(e2);
}

auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= (r1 + r2) * (r1 + r2);
};

Position* getPositionTypePtr() {
	return nullptr;
}

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
	std::cout << "Possible Threads: " << std::thread::hardware_concurrency() << std::endl;
	m_CameraController.SetZoomLevel(25.0f);

	area = Marble::CreateScope<Area2D>(Area2D({m_CameraController.GetAspectRatio() * 2 * m_CameraController.GetZoomLevel(), 1 * 2 * m_CameraController.GetZoomLevel()}));
	area->SetColorRGBA(0, 85, 130);

	rng.seed(123);

	//auto PositonID = EntityManager.AddComponent<Position>();
	//auto VelocityID = EntityManager.AddComponent<Velocity>();
	//auto MassID = EntityManager.AddComponent<Mass>();
	//auto RadiusID = EntityManager.AddComponent<Radius>();
	//auto RotationID = EntityManager.AddComponent<Rotation>();
	//auto ColorID = EntityManager.AddComponent<Color>();
	//auto CollidedID = EntityManager.AddComponent<Collided>();

	//auto PositonID2 = EntityManager.AddComponent<Position>();


	//std::cout << PositonID << std::endl;
	//std::cout << VelocityID << std::endl;
	//std::cout << MassID << std::endl;
	//std::cout << RadiusID  << std::endl;
	//std::cout << RotationID << std::endl;
	//std::cout << ColorID << std::endl;

	//std::cout << PositonID2 << std::endl;

	//ECS::HeapAllocator<1024> heapData;

	//ECS::MemBlk blk = heapData.allocate(sizeof(Position) * 3);

	//Position* positionArrPtr = static_cast<Position*>(blk.ptr);

	//Position pos1 = {1.0f, 1.0f, 1.0f};
	//Position pos2 = { 2.0f, 2.0f, 2.0f };
	//positionArrPtr[0] = pos1;
	//positionArrPtr[1] = pos2;
	//EntityManager.CreateArcheType<Position, Velocity, Mass, Radius, Rotation, Color, Collided>();

	//EntityManager.CreateArcheType<Position, Velocity, Mass, Radius, Rotation, Color>();

	//EntityManager.CreateArcheType<Position, Velocity, Mass, Radius, Rotation, Color, Collided>();

	//EntityManager.CreateArcheType<Position, Velocity, Mass, Radius, Rotation, Color, Collided>();

	////EntityManager.CreateArcheType2<Position, Velocity>();

	//EntityManager.CreateArcheType<Velocity, Position, Mass, Radius, Rotation, Color, Collided>();

	//EntityManager.CreateArcheType<>();

	std::vector<Position> positions;
	//std::vector<Velocity> positions;
	std::vector<Mass> masses;

	for (int i = 0; i < numBalls; i++){
		//m_EntityManager->CreateEntity()
		float ballMass = rng.randFloat(1, 5);

		//ECS::EntityID ballID = EntityManager.Entity();
		//
		//ids.push_back(ballID);

		//masses.push_back({ ballMass });
		//positions.push_back({ rng.randFloat(area->leftBorder, area->rightBorder), rng.randFloat(area->bottomBorder, area->topBorder), (i / (numBalls + 1.0f)) });
		//EntityManager.Add<Position>(ballID, positions[i]);
		//EntityManager.Add<Velocity>(ballID, { rng.randFloat(-3, 3), rng.randFloat(-3, 3), 0.0f });
		//EntityManager.Add<Mass>(ballID, { ballMass });
		//EntityManager.Add<Radius>(ballID, { ballMass * 0.5f });
		//EntityManager.Add<Color>(ballID, {1.0f, 1.0f, 1.0f, 1.0f});
		//EntityManager.Add<Collided>(ballID, { false });

		balls.Position[i] = { rng.randFloat(area->leftBorder, area->rightBorder), rng.randFloat(area->bottomBorder, area->topBorder), (i / (numBalls + 1.0f))};
		balls.Velocity[i] = { rng.randFloat(-3, 3), rng.randFloat(-3, 3), 0.0f };
		balls.Mass[i] = ballMass;
		balls.Radius[i]   = ballMass * 0.5f;
		balls.Color[i]	  = glm::vec4(1.0f);
		balls.collided[i] = false;
	}

	//for (int i = 0; i < numBalls; i++) {
	//	std::cout << "IDs: " << ids[i] << std::endl;
	//	/*std::cout << "OG Mass: " << masses[i].x << std::endl;
	//	Mass* getMass = EntityManager.GetPtr<Mass>(ids[i]);
	//	std::cout << "Got Mass: " << getMass->x << std::endl;

	//	std::cout << "OG Positions: " << positions[i].x << positions[i].y << positions[i].z << std::endl;*/
	//	Position* getPos = EntityManager.GetPtr<Position>(ids[i]);
	//	std::cout << "Got Positions: " << getPos->x << " " << getPos->y << " " << getPos->z << std::endl;
	//	Position getPosR = EntityManager.GetRef<Position>(ids[i]);
	//	std::cout << "Got Positions Ref: " << getPosR.x << " " << getPosR.y << " " << getPosR.z << std::endl;
	//	/*getPosR.x = 10.0f;
	//	std::cout << "Got Positions: " << getPos->x << " " << getPos->y << " " << getPos->z << std::endl;*/

	//}

	
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

	float deltaTime = ts;


	FrameCounter++;
	TimeSinceLastSecond += ts;
	if (TimeSinceLastSecond >= 1.0f){
		FramesPerSecond = FrameCounter;
		FrameCounter = 0;
		TimeSinceLastSecond = 0;
	}

	for (int i = 0; i < numBalls; i++) {
		if (balls.collided[i]){
			balls.Color[i] = glm::vec4({ 1.0f, 0.0f, 0.0f, 1.0f });
			balls.Rotation[i] = atan2f(balls.Velocity[i].y, balls.Velocity[i].x);
		} else {
			balls.Color[i] = glm::vec4(1.0f);
		}
		balls.collided[i] = false;		
	}

	for (int i = 0; i < numBalls; i++) {
		if (balls.Position[i].x >= area->rightBorder) balls.Position[i].x = area->leftBorder;
		if (balls.Position[i].x <	area->leftBorder) balls.Position[i].x = area->rightBorder;
		if (balls.Position[i].y >= area->topBorder)   balls.Position[i].y = area->bottomBorder;
		if (balls.Position[i].y < area->bottomBorder) balls.Position[i].y = area->topBorder;
	}


	//UpdatePosition(EntityManager, deltaTime);

	for (int i = 0; i < numBalls; i++) {
		balls.Position[i] += balls.Velocity[i] * deltaTime;
	}

	for (int i = 0; i < numBalls; i++) {
		if (fabs(balls.Velocity[i].x * balls.Velocity[i].x + balls.Velocity[i].y * balls.Velocity[i].y) < 0.01f) {
			balls.Velocity[i] = glm::vec3(0.0f);
		}
	}
	
	for (int i = 0; i < numBalls - 1; i++) {
		for (int j = i + 1; j < numBalls; j++) {
			if (DoCirclesOverlap(balls.Position[i].x, balls.Position[i].y, balls.Radius[i], balls.Position[j].x, balls.Position[j].y, balls.Radius[j])) {

				balls.collided[i] = true;
				balls.collided[j] = true;

				float distance = sqrtf(pow((balls.Position[i].x - balls.Position[j].x), 2.0f) + pow((balls.Position[i].y - balls.Position[j].y), 2.0f));

				float overlap = (distance - balls.Radius[i] - balls.Radius[j]) * 0.5f;

				float nx = (balls.Position[j].x - balls.Position[i].x) / distance;
				float ny = (balls.Position[j].y - balls.Position[i].y) / distance;

				balls.Position[i].x += overlap * nx;
				balls.Position[i].y += overlap * ny;
																	   
				balls.Position[j].x -= overlap * nx;
				balls.Position[j].y -= overlap * ny;
			
				float tx = -ny;
				float ty = nx;

				float dpTan1 = balls.Velocity[i].x * tx + balls.Velocity[i].y * ty;
				float dpTan2 = balls.Velocity[j].x * tx + balls.Velocity[j].y * ty;

				float dpNorm1 = balls.Velocity[i].x * nx + balls.Velocity[i].y * ny;
				float dpNorm2 = balls.Velocity[j].x * nx + balls.Velocity[j].y * ny;

				float m1 = (dpNorm1 * (balls.Mass[i] - balls.Mass[j]) + 2.0f * balls.Mass[j] * dpNorm2) / (balls.Mass[i] + balls.Mass[j]);
				float m2 = (dpNorm2 * (balls.Mass[j] - balls.Mass[i]) + 2.0f * balls.Mass[i] * dpNorm1) / (balls.Mass[i] + balls.Mass[j]);

				balls.Velocity[i].x = tx * dpTan1 + nx * m1;
				balls.Velocity[i].y = ty * dpTan1 + ny * m1;

				balls.Velocity[j].x = tx * dpTan2 + nx * m2;
				balls.Velocity[j].y = ty * dpTan2 + ny * m2;
			}
		}
	}
	
	//std::cout << ECS::Entity::Create() << std::endl;
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
			Marble::Renderer2D::DrawRotatedQuad(balls.Position[i], glm::vec2(balls.Radius[i] * 2), balls.Rotation[i], balls.m_Texture, 1.0f, balls.Color[i]);
			//cellVector[i].OnRender();
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

