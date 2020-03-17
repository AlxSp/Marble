#include "Noise2D.h"

Noise2D::Noise2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f, true)
{
}

void Noise2D::OnAttach()
{
	NC_PROFILE_FUNCTION();

	//m_FlowerTexture = Nucleus::Texture2D::Create("assets/textures/transparentFlower.png");
	m_NoiseTexture = Nucleus::Texture2D::Create(1024, 1024);
	uint32_t whiteTextureData = 0xffffffff;
	m_NoiseTexture->SetData(&NoiseMap, sizeof(float) * 128 * 128);
}

void Noise2D::OnDetach()
{
	NC_PROFILE_FUNCTION();
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

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			//glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
			NoiseMap[y][x * 4] = x * 2 ;
			NoiseMap[y][x * 4 + 1] = 51;
			NoiseMap[y][x * 4 + 2] = y * 2;
			NoiseMap[y][x * 4 + 3] = 255;
			/*
			int randInt = rng.randInt(0, 255);
			NoiseMap[y][x * 4] = randInt;
			NoiseMap[y][x * 4 + 1] = randInt;
			NoiseMap[y][x * 4 + 2] = randInt;
			NoiseMap[y][x * 4 + 3] = 255;
			*/
		}
	}   

	m_NoiseTexture->SetData(&NoiseMap, sizeof(uint8_t) * 128 * 4 * 128);

	Nucleus::BatchRenderer2D::BeginScene(m_CameraController.GetCamera());

	Nucleus::BatchRenderer2D::DrawQuad({ -.75, -.75 }, { 1.5f, 1.5f }, m_NoiseTexture);

	Nucleus::BatchRenderer2D::EndScene();
}

void Noise2D::OnImGuiRender()
{
}

void Noise2D::OnEvent(Nucleus::Event& e)
{
	m_CameraController.OnEvent(e);
}
