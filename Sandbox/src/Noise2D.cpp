#include "Noise2D.h"

#include "Timer.h"
#include "Floor.h"

#include <chrono>
#include "imgui/imgui.h"

Noise2D::Noise2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
}

void Noise2D::OnAttach()
{
	NC_PROFILE_FUNCTION();
	simplexNoise = new Nucleus::SimplexNoise();
	//NoiseMap = new uint8_t[m_Width * m_Height];
	//m_FlowerTexture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");
	m_NoiseTexture = Nucleus::Texture2D::Create(m_Width, m_Height);
	uint32_t whiteTextureData = 0xffffffff;
	m_NoiseTexture->SetData(&NoiseMap, sizeof(uint8_t) * m_Width * 4 * m_Height);
}

void Noise2D::OnDetach()
{
	NC_PROFILE_FUNCTION();
	//delete NoiseMap[];
}

void Noise2D::OnUpdate(Nucleus::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	{
		NC_PROFILE_SCOPE("Render Prep");

		Nucleus::RenderCommand::SetClearColor({ .8f, 1.0f, 1.0f, 1 });
		Nucleus::RenderCommand::Clear();
	}

	Nucleus::Lehmer32 rng = Nucleus::Lehmer32();

	/*
	std::chrono::high_resolution_clock::time_point StartTimePoint;
	std::chrono::high_resolution_clock::time_point EndTimePoint;

	double meantime = 0;
	printf("===========================================\n");
	printf("\n\nV1\n");
	{
		Timer timer;
		for (float i = -1500.0f; i < 1500.0f; i += .25) {

			uint32_t val = fastfloor1(i);
			//printf("Float value %f Floor value %i\n", i, fastfloor3(i));
		}
	}

	printf("\n\nV2\n");
	{
		Timer timer;
		for (float i = -1500.0f; i < 1500.0f; i += .25) {

			uint32_t val = fastfloor2(i);
			//printf("Float value %f Floor value %i\n", i, fastfloor3(i));
		}
	}

	printf("\n\nV3\n");
	{
		Timer timer;
		for (float i = -1500.0f; i < 1500.0f; i += .25) {

			uint32_t val = fastfloor3(i);
			//printf("Float value %f Floor value %i\n", i, fastfloor3(i));
		}
	}
	*/

	/*for (int i = 0; i < m_Width; i++) {

		for (int j = 0; j < m_Height; j++) {
			glm::vec4 color = { i / (float)m_Width, 0.2f, j / (float)m_Height, 1.0f };

			uint8_t r = (uint8_t)(color.r * 255);
			uint8_t g = (uint8_t)(color.g * 255);
			uint8_t b = (uint8_t)(color.b * 255);
			uint8_t a = (uint8_t)(color.a * 255);

			NoiseMap[i][j][0] = (uint8_t)(color.r * 255);
			NoiseMap[i][j][1] = (uint8_t)(color.g * 255);						 
			NoiseMap[i][j][2] = (uint8_t)(color.b * 255);
			NoiseMap[i][j][3] = 255;

		}

	}*/
	{
		//float max = -1000000.0f;
		//float min = 1000000.0f;

		float increment = 1.00f;
		//float increment = 0.01f;
		//m_Frequency = 0.01;

		Timer timer;
			for (int x = 0; x < m_Width; x++) {
				for (int y = 0; y < m_Height; y++) {
					float value = (Nucleus::SimplexNoise::extrapolate(x + addXoffseet, y + 50, (size_t)m_Octaves, m_Frequency, m_Amplitude, m_Lacunarity, m_Persistence));
					//float value = (Nucleus::SimplexNoise::Noise(x * increment + addXoffseet, y * increment));
					value = (value + 1) / 2 * 255;

					NoiseMap[x][y][0] = (uint8_t)value;
					NoiseMap[x][y][1] = (uint8_t)value;
					NoiseMap[x][y][2] = (uint8_t)value;
					NoiseMap[x][y][3] = (uint8_t)255;


					
					/*int randInt = rng.randInt(0, 255);
					NoiseMap[x][y][0] = (uint8_t)randInt;
					NoiseMap[x][y][1] = (uint8_t)randInt;
					NoiseMap[x][y][2] = (uint8_t)randInt;
					NoiseMap[x][y][3] = (uint8_t)255;*/
					
				}
			}
			//std::cout << "Max: " << max << std::endl;
			//std::cout << "Min: " << min << std::endl;
			//addXoffseet += 0.01f;
	}
	m_NoiseTexture->SetData(&NoiseMap, sizeof(uint8_t) * 128 * 4 * 128);

	Nucleus::BatchRenderer2D::BeginScene(m_CameraController.GetCamera());

	//Nucleus::BatchRenderer2D::SetStandardLayout();

	Nucleus::BatchRenderer2D::DrawQuad({ -.5, 0.0f }, { 1.f, 1.f }, m_NoiseTexture);

	Nucleus::BatchRenderer2D::EndScene();


	Nucleus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Nucleus::Renderer2D::DrawQuad({ 1.2f, 0.5f}, { 1.f, 1.f }, m_NoiseTexture);

	Nucleus::Renderer2D::EndScene();
}

void Noise2D::OnImGuiRender()
{
	ImGui::Begin("Simplex Noise");
	ImGui::SliderInt("Octaves", &m_Octaves, 1, 10);
	ImGui::SliderFloat("Frequency", &m_Frequency, 0.0f, 1.0f);
	ImGui::SliderFloat("Amplitude", &m_Amplitude, 0.0f, 3.0f);
	ImGui::SliderFloat("Lacunarity", &m_Lacunarity, 0.0f, 10.0f);
	ImGui::SliderFloat("Persistence", &m_Persistence, 0.05f, 1.0f);
	//ImGui::Text("FPS: %i", FPS);
	//ImGui::Text("X: %f \t Y: %f", player.Position.x, player.Position.y);

	//ColorEdit4("Square", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Noise2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}
