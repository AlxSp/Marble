#include "Sandbox2D.h"
#include "imgui.h"
#include "RendererAPI/OpenGL/OpenGLShader.h"
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

	// operator glm::vec3() const {

	// }
};

struct Velocity {
	float x;
	float y;
	float z;
};

struct Mass {
	float value;
};

struct Radius {
	float value;
};

struct Rotation {
	float value;
};

struct Color {
	float r;
	float g;
	float b;
	float a;
};

struct Collided {
	bool boolean;
};


void CheckAreaBounds(entt::registry& reg, Area2D& area2D) {
	reg.view<Position>().each(
		[&area2D](auto& pos){
			if (pos.x >= area2D.rightBorder) 	pos.x = area2D.leftBorder;
			else if (pos.x < area2D.leftBorder) 	pos.x = area2D.rightBorder;
			if (pos.y >= area2D.topBorder)   	pos.y = area2D.bottomBorder;
			else if (pos.y < area2D.bottomBorder) pos.y = area2D.topBorder;	
		}
	);
}

void UpdatePosition(entt::registry& reg, float& dt) {
	reg.view<Position, Velocity>().each(
		[&dt](auto& pos, auto& vel) {
			pos.x += vel.x * dt;
			pos.y += vel.y * dt;
		}
	);
}

void UpdateMinimumVelocity(entt::registry& reg) {
	reg.view<Velocity>().each(
		[](auto& vel){
			if (fabs(vel.x * vel.x + vel.y * vel.y) < 0.01f) {
				vel.x = 0.0f;
				vel.y = 0.0f;
				//balls.Velocity[i] = glm::vec3(0.0f);
			}
		}
	);
}

void UpdateCollsion(entt::registry& reg) {
	auto view = reg.view<Position, Radius>();

	for (auto entity: view) {
		auto [pos, vel] = view.get<Position, Radius>(entity);
	}
}

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


	// std::vector<Position> positions;
	// //std::vector<Velocity> positions;
	// std::vector<Mass> masses;

	//std::cout << ASSETS("textures/forward_circle.png") << std::endl;

	for (int i = 0; i < numBalls; i++){

		float ballMass = rng.randFloat(1, 5);

		auto entity = Registry.create();
		ballEntities[i] = entity;

		Registry.emplace<Position>(entity, rng.randFloat(area->leftBorder, area->rightBorder), rng.randFloat(area->bottomBorder, area->topBorder), 0.2f); 
		Registry.emplace<Velocity>(entity, rng.randFloat(-3, 3), rng.randFloat(-3, 3), 0.0f);
		Registry.emplace<Mass>(entity, ballMass);
		Registry.emplace<Rotation>(entity, 0.0f);
		Registry.emplace<Radius>(entity, ballMass * 0.5f);
		Registry.emplace<Color>(entity, 1.0f, 1.0f, 1.0f, 1.0f);
		Registry.emplace<Collided>(entity, false);

		balls.Position[i] = { rng.randFloat(area->leftBorder, area->rightBorder), rng.randFloat(area->bottomBorder, area->topBorder), (i / (numBalls + 1.0f))};
		balls.Velocity[i] = { rng.randFloat(-3, 3), rng.randFloat(-3, 3), 0.0f };
		balls.Mass[i] = ballMass;
		balls.Radius[i]   = ballMass * 0.5f;
		balls.Color[i]	  = glm::vec4(1.0f);
		balls.collided[i] = false;
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

	float deltaTime = ts;


	FrameCounter++;
	TimeSinceLastSecond += ts;
	if (TimeSinceLastSecond >= 1.0f){
		FramesPerSecond = FrameCounter;
		FrameCounter = 0;
		TimeSinceLastSecond = 0;
	}


	CheckAreaBounds(Registry, *area);

	UpdatePosition(Registry, deltaTime);

	UpdateMinimumVelocity(Registry);

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


	for (int i = 0; i < numBalls; i++) {
		balls.Position[i] += balls.Velocity[i] * deltaTime;
	}

	for (int i = 0; i < numBalls; i++) {
		if (fabs(balls.Velocity[i].x * balls.Velocity[i].x + balls.Velocity[i].y * balls.Velocity[i].y) < 0.01f) {
			balls.Velocity[i] = glm::vec3(0.0f);
		}
	}

	auto view = Registry.view<Position, Velocity, Radius, Mass, Collided>();
	
	// for(auto entity = view.begin(); entity != --view.end() ; ++entity)
	// {
	// 	auto [pos, vel, rad, mass, coll] = view.get<Position, Velocity, Radius, Mass, Collided>(*entity);
		
	// 	for(auto entitySecond = entity; entitySecond != view.end() ; ++entitySecond)
	// 	{
	// 		if (entity == entitySecond){
	// 			continue;
	// 		}

	// 		auto [posSecond, velSecond, radSecond, massSecond, collSecond] = view.get<Position, Velocity, Radius, Mass, Collided>(*entitySecond);
			
	// 		if (DoCirclesOverlap(pos.x, pos.y, rad.value, posSecond.x, posSecond.y, radSecond.value)) {
				
	// 			coll.boolean = true;
	// 			collSecond.boolean = true;

	// 			float distance = sqrtf(pow((pos.x - posSecond.x), 2.0f) + pow((pos.y - posSecond.y), 2.0f));

	// 			float overlap = (distance - rad.value - radSecond.value) * 0.5f;

	// 			float nx = (posSecond.x - pos.x) / distance;
	// 			float ny = (posSecond.y - pos.y) / distance;

	// 			pos.x += overlap * nx;
	// 			pos.y += overlap * ny;
																	   
	// 			posSecond.x -= overlap * nx;
	// 			posSecond.y -= overlap * ny;
			
	// 			float tx = -ny;
	// 			float ty = nx;

	// 			float dpTan1 = vel.x * tx + vel.y * ty;
	// 			float dpTan2 = velSecond.x * tx + velSecond.y * ty;

	// 			float dpNorm1 = vel.x * nx + vel.y * ny;
	// 			float dpNorm2 = velSecond.x * nx + velSecond.y * ny;

	// 			float m1 = (dpNorm1 * (mass.value - massSecond.value) + 2.0f * massSecond.value * dpNorm2) / (mass.value + massSecond.value);
	// 			float m2 = (dpNorm2 * (massSecond.value - mass.value) + 2.0f * mass.value * dpNorm1) / (mass.value + massSecond.value);

	// 			vel.x = tx * dpTan1 + nx * m1;
	// 			vel.y = ty * dpTan1 + ny * m1;

	// 			vel.x = tx * dpTan2 + nx * m2;
	// 			vel.y = ty * dpTan2 + ny * m2;
	// 		}
			
	// 	}	
	// }
	
	// for (int i = 0; i < numBalls - 1; i++) {
	// 	for (int j = i + 1; j < numBalls; j++) {
	// 		if (DoCirclesOverlap(balls.Position[i].x, balls.Position[i].y, balls.Radius[i], balls.Position[j].x, balls.Position[j].y, balls.Radius[j])) {

	// 			balls.collided[i] = true;
	// 			balls.collided[j] = true;

	// 			float distance = sqrtf(pow((balls.Position[i].x - balls.Position[j].x), 2.0f) + pow((balls.Position[i].y - balls.Position[j].y), 2.0f));

	// 			float overlap = (distance - balls.Radius[i] - balls.Radius[j]) * 0.5f;

	// 			float nx = (balls.Position[j].x - balls.Position[i].x) / distance;
	// 			float ny = (balls.Position[j].y - balls.Position[i].y) / distance;

	// 			balls.Position[i].x += overlap * nx;
	// 			balls.Position[i].y += overlap * ny;
																	   
	// 			balls.Position[j].x -= overlap * nx;
	// 			balls.Position[j].y -= overlap * ny;
			
	// 			float tx = -ny;
	// 			float ty = nx;

	// 			float dpTan1 = balls.Velocity[i].x * tx + balls.Velocity[i].y * ty;
	// 			float dpTan2 = balls.Velocity[j].x * tx + balls.Velocity[j].y * ty;

	// 			float dpNorm1 = balls.Velocity[i].x * nx + balls.Velocity[i].y * ny;
	// 			float dpNorm2 = balls.Velocity[j].x * nx + balls.Velocity[j].y * ny;

	// 			float m1 = (dpNorm1 * (balls.Mass[i] - balls.Mass[j]) + 2.0f * balls.Mass[j] * dpNorm2) / (balls.Mass[i] + balls.Mass[j]);
	// 			float m2 = (dpNorm2 * (balls.Mass[j] - balls.Mass[i]) + 2.0f * balls.Mass[i] * dpNorm1) / (balls.Mass[i] + balls.Mass[j]);

	// 			balls.Velocity[i].x = tx * dpTan1 + nx * m1;
	// 			balls.Velocity[i].y = ty * dpTan1 + ny * m1;

	// 			balls.Velocity[j].x = tx * dpTan2 + nx * m2;
	// 			balls.Velocity[j].y = ty * dpTan2 + ny * m2;
	// 		}
	// 	}
	// }

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
		auto renderView = Registry.view<Position, Radius, Rotation, Color>();
		for (auto entity: renderView){
			auto [pos, rad, rot, color] = renderView.get<Position, Radius, Rotation, Color>(entity);
			Marble::Renderer2D::DrawRotatedQuad(reinterpret_cast<glm::vec3&>(pos), glm::vec2(rad.value * 2), rot.value, balls.m_Texture, 1.0f, reinterpret_cast<glm::vec4&>(color));
			//std::cout << yr << ": " << pos.x << " " << pos.y << " " << pos.z << std::endl; 
		}

		// std::cout << "Reg render:" << std::endl;
		// for (int i = 0; i < numBalls; i++) {
		// 	std::cout << i << ": " << balls.Position[i].x << " " << balls.Position[i].y << " " << balls.Position[i].z << std::endl; 
		// 	// Marble::Renderer2D::DrawRotatedQuad(balls.Position[i], glm::vec2(balls.Radius[i] * 2), balls.Rotation[i], balls.m_Texture, 1.0f, balls.Color[i]);
		// }
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

