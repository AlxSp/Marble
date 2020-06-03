#include "EditorComponent.h"

template<> void EditorComponent<Position>::ShowInfo(entt::registry& registry, entt::entity& entity) {
	auto& component = registry.get<Position>(entity);
	ImGui::SliderFloat("x", &component.x, -10.f, 10.f);
	ImGui::SliderFloat("y", &component.y, -10.f, 10.f);
	ImGui::SliderFloat("z", &component.z, -10.f, 10.f);
}