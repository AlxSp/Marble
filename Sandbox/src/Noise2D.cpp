#include "Noise2D.h"


Noise2D::Noise2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
}

void Noise2D::OnAttach()
{
	NC_PROFILE_FUNCTION();
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

	for (int x = 0; x < m_Width; x++) {
		for (int y = 0; y < m_Height; y++) {
			int randInt = rng.randInt(0, 255);
			NoiseMap[x][y][0] = (uint8_t)randInt;
			NoiseMap[x][y][1] = (uint8_t)randInt;
			NoiseMap[x][y][2] = (uint8_t)randInt;
			NoiseMap[x][y][3] = (uint8_t)255;
		}
	}   

	m_NoiseTexture->SetData(&NoiseMap, sizeof(uint8_t) * 128 * 4 * 128);

	Nucleus::BatchRenderer2D::BeginScene(m_CameraController.GetCamera());

	Nucleus::BatchRenderer2D::DrawQuad({ -.5, 0.0f }, { 1.f, 1.f }, m_NoiseTexture);

	Nucleus::BatchRenderer2D::EndScene();


	Nucleus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Nucleus::Renderer2D::DrawQuad({ 1.2f, 0.5f}, { 1.f, 1.f }, m_NoiseTexture);

	Nucleus::Renderer2D::EndScene();
}

void Noise2D::OnImGuiRender()
{
}

void Noise2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}
