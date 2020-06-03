#pragma once
#include "Components.h"
#include <entt/entt.hpp>
#include "imgui.h"

struct BaseEditorComponent {
    BaseEditorComponent(std::string name) : Name(name) {}
    virtual void Add(entt::registry& registry, entt::entity& entity) = 0;
    virtual void ShowInfo(entt::registry& registry, entt::entity& entity) = 0;
    std::string Name;
};

template <typename T>
struct EditorComponent : public BaseEditorComponent {
    EditorComponent(std::string name) : BaseEditorComponent(name) {}
    virtual void Add(entt::registry& registry, entt::entity& entity) override { registry.emplace<T>(entity);};
    virtual void ShowInfo(entt::registry& registry, entt::entity& entity) override { ImGui::Text("This Component does not have a defined UI"); };
};