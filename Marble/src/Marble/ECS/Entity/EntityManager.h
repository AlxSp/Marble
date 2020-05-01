#pragma once
#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/ComponentSystem.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"

#include <memory>
#include <typeinfo> 
#include <typeindex>

namespace ECS {

    class family {
    public:
        template<typename>
        static uint64_t getID() noexcept {
            static const uint64_t id = identifier();
            return id;
        }
    private:
        static uint64_t identifier() noexcept {
            static uint64_t value = 1;
            return value++;
        }
    };



    struct ComponentArray {
        void* elements;
        size_t size;
    };

    struct ArcheType;
    struct GraphEdge {
        ArcheType* add = nullptr;
        ArcheType* remove = nullptr;
    };

    struct ArcheType {
        Type type;
        std::vector<ComponentArray> components;
        std::vector<EntityID> entityIDs;
        size_t length = 0;

        std::vector<GraphEdge> edges;
    };

    static void createArcheType(ArcheType* previousArcheType, ComponentID& id) {
        //return new ArcheType()
    }

    static void moveEntityToArcheType() {}

    struct Record {
        ArcheType* archetype;
        int rowIndex;
    };


    class EntityManager 
    {
    public:
        EntityManager() { Root = new ArcheType(); }
        EntityManager(const EntityManager&) { delete Root; }

        EntityID Entity() {
            EntityIndex.insert({ m_EntityIndex, {Root, 0} });
            return m_EntityIndex++;
        }

        template<typename T>
        void Add(EntityID& entityID, T type) {
            Record rec = EntityIndex.at(entityID);
            ArcheType* previousArcheType = rec.archetype;
            Type& type = r.archetype->type;

            for (int i = 0; i < type.size(); i++) {
                if (type[i] == A) {
                    return r.archetype->components[i].elements[r.row];
                }
            }
        }   

        template<typename T>
        ComponentID Component(const std::string& name) {
            return family::getID<T>();
        }
        //void DeleteComponent(ComponentID id);
        
        template<typename ...T>
        void System() {}



    private:
        EntityID    m_EntityIndex = 1;
        ComponentID m_ComponentIndex = 1;

        ECS::HeapAllocator<1024 * 1024 * 1024> heapAlloc;

        ArcheType* Root;
        Type AllTypes;
        std::unordered_map<EntityID, Record> EntityIndex;
    };
}