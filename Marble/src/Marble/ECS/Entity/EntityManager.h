#pragma once
#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/ComponentSystem.h"
#include "Marble/ECS/MemoryAllocator.h"

namespace ECS {

    class Component;

    


    class Entity {
    public:
        Entity(const Entity&) = delete;
        static Entity& Get() { return s_Instance; }

        static EntityID Create() { return Get().InternalCreate();}
    private:
        Entity() {}
        EntityID InternalCreate() {return ++Index;}

        EntityID Index = 1;
    private:
        static Entity s_Instance; 
    };

    class EntityManager 
    {
    public:
        EntityManager() {
            
        };
        EntityManager(const EntityManager&) = delete;

        template<typename T>
        ComponentID Component(const std::string& name) {
            std::vector<T> test;
            return m_ComponentIndex++;
        }
        void DeleteComponent(ComponentID id);
        
    private:
        EntityID    m_EntityIndex = 1;
        ComponentID m_ComponentIndex = 1;

        //PoolAllocator Allocator;

        //static EntityManager s_Instance;
    };
}