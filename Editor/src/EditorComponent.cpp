#include "EditorComponent.h"

#include <limits>

template<> void EditorComponent<Position>::ShowInfo(entt::registry& registry, entt::entity& entity) {
	auto& component = registry.get<Position>(entity);

	ImGui::BeginChild("x", ImVec2(155, 35));
	ImGui::InputFloat("x", &component.x, .01f, .1f, 3);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("y", ImVec2(155, 35));
	ImGui::InputFloat("y", &component.y, .01f, .1f, 3);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("z", ImVec2(155, 35));
	ImGui::InputFloat("z", &component.z, .01f, .1f, 3);
	ImGui::EndChild();
}

template<> void EditorComponent<Size2D>::ShowInfo(entt::registry& registry, entt::entity& entity) {
	auto& component = registry.get<Size2D>(entity);
	ImGui::BeginChild("x", ImVec2(155, 35));
	ImGui::InputFloat("x", &component.x, .01f, .1f, 3);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("y", ImVec2(155, 35));
	ImGui::InputFloat("y", &component.y, .01f, .1f, 3);
	ImGui::EndChild();
}

template<> void EditorComponent<ColorRGBA>::ShowInfo(entt::registry& registry, entt::entity& entity) {
	auto& component = registry.get<ColorRGBA>(entity);
	ImGui::BeginChild("x", ImVec2(155, 35));
	ImGui::InputFloat("x", &component.r, .01f, .1f, 3);
	if (component.r > 1.0f)
		component.r = 1.0f;
	else if (component.r < 0.0f)
		component.r = 0.0f;
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("y", ImVec2(155, 35));
	ImGui::InputFloat("y", &component.g, .01f, .1f, 3);
	if (component.g > 1.0f)
		component.g = 1.0f;
	else if (component.g < 0.0f)
		component.g = 0.0f;
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("z", ImVec2(155, 35));
	ImGui::InputFloat("z", &component.b, .01f, .1f, 3);
	if (component.b > 1.0f)
		component.b = 1.0f;
	else if (component.b < 0.0f)
		component.b = 0.0f;
	ImGui::EndChild();
}