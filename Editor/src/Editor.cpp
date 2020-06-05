#include "Editor.h"

#include "imgui.h"
#include "RendererAPI/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_internal.h>

#include <chrono>
#include <random>

Editor::Editor() 
	: Layer("Editor"), m_CameraController(1920.0f / 1080.0f, true)
{
	EditorRenderBuffer->Bind();
	EditorRenderBuffer->AttachTexture2D(EditorRenderTexture->GetRenderID());
	EditorRenderBuffer->Unbind();

	Components.push_back(new EditorComponent<Position>("Position"));
	Components.push_back(new EditorComponent<Size2D>("Size2D"));
	Components.push_back(new EditorComponent<ColorRGBA>("ColorRGBA"));
	Components.push_back(new EditorComponent<Renderable>("Renderable"));
}

Editor::~Editor(){
	
	std::ostringstream storage;

	auto view = Registry.view<Position, Size2D, ColorRGBA, Renderable>();

	for(auto entity: view) {
		//Marble::Renderer2D::DrawQuad(reinterpret_cast<glm::vec3&>(view.get<Position>(entity)), reinterpret_cast<glm::vec2&>(view.get<Size2D>(entity)), reinterpret_cast<glm::vec4&>(view.get<ColorRGBA>(entity)));
		
		//memcpy(storage, )
		//storage << (view.get<Position>(entity));
		
		//std::cout << "Found Entity" << std::endl;
	}
	//entt::snapshot{Registry}.entities(nullptr); //.component<Position>()


}

void Editor::OnAttach()
{
	MBL_PROFILE_FUNCTION();
}

void Editor::OnDetach()
{
	MBL_PROFILE_FUNCTION();
}

void Editor::OnUpdate(Marble::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);
	Marble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Marble::RenderCommand::Clear();


	EditorRenderBuffer->Bind();
	MBL_PROFILE_FUNCTION();
	{
		MBL_PROFILE_SCOPE("Render Prep");
		Marble::RenderCommand::SetViewport(0, 0, ViewPortWidth, ViewPortHeight);
		Marble::RenderCommand::SetClearColor({ 0.0f, 0.2f, 0.4f, 1 });
		Marble::RenderCommand::Clear();
	}
	{
		MBL_PROFILE_SCOPE("Render Draw");
		Marble::Renderer2D::ResetStats();
		Marble::Renderer2D::BeginScene(m_CameraController.GetCamera());

		auto view = Registry.view<Position, Size2D, ColorRGBA, Renderable>();

		for(auto entity: view) {
			Marble::Renderer2D::DrawQuad(reinterpret_cast<glm::vec3&>(view.get<Position>(entity)), reinterpret_cast<glm::vec2&>(view.get<Size2D>(entity)), reinterpret_cast<glm::vec4&>(view.get<ColorRGBA>(entity)));
			//std::cout << "Found Entity" << std::endl;
		}

		Marble::Renderer2D::EndScene();
	}
	EditorRenderBuffer->Unbind();
}

void Editor::OnImGuiRender()
{
	ImVec2 MainViewPortPosition = ImGui::GetMainViewport()->Pos;
	ImVec2 MainViewPortSize = ImGui::GetMainViewport()->Size; 

	MBL_PROFILE_FUNCTION();
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	ImGui::BeginMainMenuBar();
	ImGui::Text("Test");          
	ImGui::Text("Test2");
	float mainBarHeight = ImGui::GetWindowHeight();
	ImGui::EndMainMenuBar();

	//Render Left Side Panel Window
	ImGui::SetNextWindowPos(MainViewPortPosition + ImVec2(0, mainBarHeight));
	ImGui::SetNextWindowSize({MainViewPortSize.x * .25f, MainViewPortSize.y - mainBarHeight});
	ImGui::Begin("Left Side Panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGui::BeginChild("Scene", ImVec2(0, 35), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Text("Scene");
	if (ImGui::Button("New Entity"))
	{
		EntityVector.push_back(EditorEntity(Registry.create(), "Entity_" + std::to_string(EntityCount)));
		++EntityCount;
	}
	ImGui::EndChild();
	//Display created entities
	ImGui::BeginChild("Entity View", ImVec2(0, MainViewPortSize.y - mainBarHeight - 35));
	
	for (int i = 0; i < EntityVector.size(); i++){
		bool entityNodeExpanded = false;
		if (i == selectedEntityIndex){
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
			entityNodeExpanded = ImGui::TreeNodeEx(EntityVector[i].Name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
			ImGui::PopStyleColor();
		}
		else {
			entityNodeExpanded = ImGui::TreeNodeEx(EntityVector[i].Name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
		}

		bool toggled = ImGui::IsItemToggledOpen();
		
		bool clicked = ImGui::IsItemClicked();
		if (ImGui::IsItemClicked()){
			selectedEntityIndex = i;
		}

		if (entityNodeExpanded) {
			for (auto& componentId : EntityVector[i].ComponentIDs){
				//ImGui::Indent();
				ImGui::Text(Components[componentId]->Name.c_str());
				//ImGui::Unindent();
			}
			ImGui::TreePop();
		}
	}
	ImGui::EndChild();
	ImGui::End();



	ImGui::SetNextWindowPos(MainViewPortPosition + ImVec2(MainViewPortSize.x -(MainViewPortSize.x * .25f), mainBarHeight));
	ImGui::SetNextWindowSize({MainViewPortSize.x * .25f, MainViewPortSize.y - mainBarHeight});
	if (selectedEntityIndex == -1){
		ImGui::Begin("No Entity Selected", nullptr, ImGuiWindowFlags_NoCollapse);
	}
	else {
		ImGui::Begin(EntityVector[selectedEntityIndex].Name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);
		bool expandedComponet = false;
		for (auto& id : EntityVector[selectedEntityIndex].ComponentIDs ){
			expandedComponet = ImGui::TreeNodeEx(Components[id]->Name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);
			if (expandedComponet) {

				Components[id]->ShowInfo(Registry, EntityVector[selectedEntityIndex].ID);

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Add Component"))
		{
			addComponent = !addComponent; 
		}
		static int selectedItem = -1;
		if (addComponent){
			ImGui::ListBoxHeader("Available Components");
			for (int i = 0; i < Components.size(); i++){
				if(std::find(EntityVector[selectedEntityIndex].ComponentIDs.begin(), EntityVector[selectedEntityIndex].ComponentIDs.end(), i) == EntityVector[selectedEntityIndex].ComponentIDs.end()) 
				{
					if (ImGui::Selectable(Components[i]->Name.c_str())){

						Components[i]->Add(Registry, EntityVector[selectedEntityIndex].ID);
						EntityVector[selectedEntityIndex].ComponentIDs.push_back(i);

						addComponent = !addComponent;
					}
				}
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::End();




	ImGui::SetNextWindowPos(MainViewPortPosition + ImVec2(MainViewPortSize.x * .25f, mainBarHeight));
	ImGui::SetNextWindowSize({MainViewPortSize.x * .5f, MainViewPortSize.x * .5f * 9 / 16});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
	ImGui::Begin("Scene ViewPort", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);

	vMin = ImGui::GetWindowContentRegionMin();
	vMax = ImGui::GetWindowContentRegionMax();

	ImVec2 viewportsize = vMax - vMin;

	ViewPortWidth = viewportsize.x;
	ViewPortHeight = viewportsize.y;
	
	ImGui::Image((void*)(intptr_t)EditorRenderTexture->GetRenderID(), viewportsize,  ImVec2(0, viewportsize.x/1920), ImVec2(viewportsize.y/1080, 0));
	ImGui::End();
	ImGui::PopStyleVar();
}

void Editor::OnEvent(Marble::Event& e)
{
	m_CameraController.OnEvent(e);
}

