#pragma once
#include "Nucleus.h"

class Container2D {
public: 
	Container2D(const glm::vec2& postion, const glm::vec2& size) : Position(postion), Size(size) {}
	~Container2D() = default;
protected:
	glm::vec2 Position;
	glm::vec2 Size;
};

class VisibleContainer2D : public Container2D {
public:
	VisibleContainer2D(const glm::vec2& postion, const glm::vec2& size) : Container2D(postion, size) {}
	~VisibleContainer2D() = default;

	//glm::vec2 GetPostion() { return Position; }
	virtual void OnRender() = 0;
	//virtual 
};



class NoiseContainer2D : public VisibleContainer2D {
public:
	NoiseContainer2D(const glm::vec2& postion, const glm::vec2& size, const uint32_t& detail);
	~NoiseContainer2D() { delete TexturePixels; }

	void SetSimplexNoiseValues(const uint8_t& octaves, const float& frequency, const float& amplitude, const float& lacunarity, const float& persistence) { 
		m_Octaves = octaves;
		m_Frequency = frequency;
		m_Amplitude = amplitude;
		m_Lacunarity = lacunarity;
		m_Persistence = persistence;
	}

	glm::vec2 GetPostion() { return Position; }
	
	void FillPixels(float xOffset = 0.0f, float yOffset = 0.0f);
	void FillPixels(glm::vec2& offset);
	void FillChannel(int& channel, float xOffset = 0.0f, float yOffset = 0.0f);

	void OnUpdate(Nucleus::TimeStep ts) {}
	void OnRender() override { Nucleus::BatchRenderer2D::DrawQuad(Position - Size / 2.0f, Size, NoiseTexture); }

private:
	uint8_t	m_Octaves = 4;
	float m_Frequency = .1f;
	float m_Amplitude = 1.0f;
	float m_Lacunarity = 2.0f;
	float m_Persistence = 0.5f;
private:
	glm::vec2 StepSize = { 1.0f, 1.0f };
	uint8_t* TexturePixels;
	uint32_t Detail;
	uint8_t Channels = 4;
	Nucleus::Ref<Nucleus::Texture2D> NoiseTexture;
};

class LevelofDetailContainer : public Container2D {
public:
	LevelofDetailContainer(const glm::vec2& postion, const glm::vec2& size, const uint32_t& detail)
		: Container2D(postion, size), Detail(detail) {
		SubContainerSize = Size.x / Detail;
		NumberOfContainers = Detail * Detail;

		//SubContainers = new VisibleContainer2D*[NumberOfContainers];
		//int detail = 128;
		for (int i = 0; i < Detail; i++) {
			for (int j = 0; j < Detail; j++) {
				SubContainers.push_back(new NoiseContainer2D({ i * SubContainerSize , j * SubContainerSize }, { SubContainerSize, SubContainerSize }, 128));
			}
		}

		for (int i = 0; i < Detail * Detail; i++) {
			SubContainers[i]->FillPixels(SubContainers[i]->GetPostion());
		}

	}

	~LevelofDetailContainer() {
		for (int i = 0; i < Detail * Detail; i++) {
			delete SubContainers[i];
		}
	}

	void SetPlayerPosition(const glm::vec2& postion) { PlayerPostion = postion; }

	void OnRender() {
		for (int i = 0; i < Detail * Detail; i++) {
			SubContainers[i]->OnRender();
		}
		
		//for (int i = 0; i < NumberOfContainers)
	}

protected:
	uint32_t Detail;
	uint32_t NumberOfContainers = 1;
	float SubContainerSize = 1.0f;
	std::vector<NoiseContainer2D*> SubContainers;
	glm::vec2 PlayerPostion;
};