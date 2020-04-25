#pragma once

class ComponentSystem
{
public:
    virtual ~ComponentSystem() = default;
    virtual bool Init() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
};