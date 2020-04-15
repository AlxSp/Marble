#include "Noise2D.h"

#include "Timer.h"
#include "Floor.h"

#include <chrono>
#include "imgui/imgui.h"

Noise2D::Noise2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
}

void Noise2D::OnAttach() {
	NC_PROFILE_FUNCTION();
	//m_CameraController.SetZoomLevel(10.0f);

	float aspectRatio = m_CameraController.GetAspectRatio();

	glm::vec2 test = { 2.333, -.5 };

	test = glm::round(test);

	float zoomDifference = .1f;

	m_Player = Nucleus::CreateRef<Player>(Player(m_CameraController.GetPosition(), m_CameraController.GetAspectRatio(), m_CameraController.GetZoomLevel()));

	perspective_width = aspectRatio * 2 * zoomDifference;// *zoomLevel;
	//perspective_height = 1 * 2;// *zoomLevel;

	ocm = new ObjectContainerManager2D(m_Player, { perspective_width, perspective_width });

	ocm->SetZoomDifference(zoomDifference);
	//ocm->SetPlayerView(m_CameraController.GetAspectRatio(), m_CameraController.GetZoomLevel() * ZoomDifference);
	//int channel = 0;

	//texture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");
}

void Noise2D::OnDetach()
{
	NC_PROFILE_FUNCTION();
	//delete NoiseMap[];
}

void Noise2D::OnUpdate(Nucleus::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);
	if (m_CameraController.GetZoomLevel() > 10) {
		m_CameraController.SetZoomLevel(10);
	}
	m_Player->SetCameraPosition(m_CameraController.GetPosition());
	m_Player->SetZoomLevel(m_CameraController.GetZoomLevel());

	//ocm->GenerateContainers();
	ocm->OnUpdate();
	{
		NC_PROFILE_SCOPE("Render Prep");

		Nucleus::RenderCommand::SetClearColor({ .8f, 1.0f, 1.0f, 1 });
		Nucleus::RenderCommand::Clear();
	}

	Nucleus::Lehmer32 rng = Nucleus::Lehmer32();

	Nucleus::BatchRenderer2D::BeginScene(m_CameraController.GetCamera());

	ocm->OnRender();


	Nucleus::BatchRenderer2D::EndScene();
}

void Noise2D::OnImGuiRender()
{

	ocm->OnImGuiRender();
	ImGui::Begin("Simplex Noise");
	ImGui::SliderInt("Octaves", &m_Octaves, 1, 10);
	ImGui::SliderFloat("Frequency", &m_Frequency, 0.0f, 1.0f);
	ImGui::SliderFloat("Amplitude", &m_Amplitude, 0.0f, 3.0f);
	ImGui::SliderFloat("Lacunarity", &m_Lacunarity, 0.0f, 10.0f);
	ImGui::SliderFloat("Persistence", &m_Persistence, 0.05f, 1.0f);
	ImGui::End();

	ImGui::Begin("Camera Controller Info");
	ImGui::SliderFloat("Perspectiver Width", &perspective_width, 0.0f, 10);
	ImGui::SliderFloat("Perspectiver Heigth", &perspective_height, 0.0f,10);
	ImGui::Text("Zoomlevel: %f", m_CameraController.GetZoomLevel());
	//ImGui::Text("X: %f \t Y: %f", player.Position.x, player.Position.y);

	//ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Noise2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}
