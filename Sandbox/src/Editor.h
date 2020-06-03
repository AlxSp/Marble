#pragma once

#include "Marble.h"
#include "EditorComponent.h"
#include <entt/entt.hpp>
#include "RendererAPI/OpenGL/OpenGLBuffer.h"
#include "AssetPath.h"

#include <filesystem>

#define MaxBalls 50

class Editor : public Marble::Layer {
public:
	Editor(); 
	virtual ~Editor(); // = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Marble::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Marble::Event& e) override;

	//Editor related
	static uint32_t SelectedElementIndex;

	struct EditorEntity {
		EditorEntity(entt::entity&& id, std::string&& name) : ID(id), Name(name) {}
		entt::entity ID;
		std::string  Name;
		std::vector<uint32_t> ComponentIDs;
	};

	uint32_t EntityCount = 0;
	int32_t selectedEntityIndex = -1;

private:
	std::filesystem::path ProjectPath = "/home/Alex/dev/MarbleDev/Projects/Path";

	Marble::OrthographicCameraController m_CameraController;


	entt::registry Registry;
	std::vector<EditorEntity> EntityVector;
	std::vector<BaseEditorComponent*> Components;// = {"Position"};


	//Scene ViewPort Variables
	Marble::Ref<Marble::FrameBuffer> EditorRenderBuffer = Marble::FrameBuffer::Create();
	Marble::Ref<Marble::Texture2D> EditorRenderTexture = Marble::Texture2D::Create(1920, 1080);

	uint32_t ViewPortWidth  = 1920;
	uint32_t ViewPortHeight = 1080;


	//Right Panel Constant Variables
	bool addComponent = false;



	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

	struct ImGUIPanel {

	};

};