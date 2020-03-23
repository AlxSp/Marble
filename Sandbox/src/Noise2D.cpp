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
	m_CameraController.SetZoomLevel(10.0f);

	float aspectRatio = m_CameraController.GetAspectRatio();
	float zoomLevel = m_CameraController.GetZoomLevel();

	perspective_width = aspectRatio * 2 * zoomLevel;
	perspective_height = 1 * 2 * zoomLevel;

	//testContainer = new NoiseContainer2D({ perspective_width * -0.5f, perspective_height * -0.5f }, { perspective_height, perspective_height }, (uint32_t)128);
	//testContainer2 = new NoiseContainer2D({ 0.0f, perspective_height * -0.5f }, { perspective_height, perspective_height }, (uint32_t)128);

	ocm = new ObjectContainerManager2D(m_CameraController.GetPosition(), { perspective_width, perspective_width });
	ocm->SetPlayerView(m_CameraController.GetAspectRatio(), m_CameraController.GetZoomLevel());
	//testContainer->FillPixels(testContainer->GetPostion());
	//testContainer2->FillPixels(testContainer2->GetPostion());
	int chanel = 0;
	//testContainer->fillChannel(chanel, 5.0f);
	//simplexNoise = new Nucleus::SimplexNoise();
	//NoiseMap = new uint8_t[m_Width * m_Height];
	//m_FlowerTexture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");
	//m_NoiseTexture = Nucleus::Texture2D::Create(m_Width, m_Height);
	//uint32_t whiteTextureData = 0xffffffff;
	//m_NoiseTexture->SetData(&NoiseMap, sizeof(uint8_t) * m_Width * 4 * m_Height);

	
}

void Noise2D::OnDetach()
{
	NC_PROFILE_FUNCTION();
	//delete NoiseMap[];
}

void Noise2D::OnUpdate(Nucleus::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	ocm->SetPlayerView(m_CameraController.GetAspectRatio(), m_CameraController.GetZoomLevel());
	ocm->SetPlayerPosition(m_CameraController.GetPosition());
	
	{
		NC_PROFILE_SCOPE("Render Prep");

		Nucleus::RenderCommand::SetClearColor({ .8f, 1.0f, 1.0f, 1 });
		Nucleus::RenderCommand::Clear();
	}

	Nucleus::Lehmer32 rng = Nucleus::Lehmer32();

	Nucleus::BatchRenderer2D::BeginScene(m_CameraController.GetCamera());

	ocm->OnRender();
	//testContainer->OnRender();
	//testContainer2->OnRender();

	Nucleus::BatchRenderer2D::EndScene();
}

void Noise2D::OnImGuiRender()
{
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
