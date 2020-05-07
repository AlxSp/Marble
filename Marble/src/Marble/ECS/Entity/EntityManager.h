#pragma once

#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/ComponentSystem.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"

#include <iostream>
#include <memory>
#include <typeinfo> 
#include <typeindex>

namespace ECS {

    namespace {
        size_t TypeBlockSize = 4096;

    }

    class Componentfamily {
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

    class family {
    public:
        template<typename ...>
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
        std::vector<MemBlk> components;
        std::vector<EntityID> entityIDs;
        size_t length = 0;

        std::unordered_map<ComponentID, GraphEdge> edges;
    };

    //static void createArcheType(ArcheType* previousArcheType, ComponentID& id) {
    //    //return new ArcheType()
    //}

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
        bool compare(std::vector<T>& v1, std::vector<T>& v2) {
            std::sort(v1.begin(), v1.end());
            std::sort(v2.begin(), v2.end());
            return v1 == v2;
        }

        ArcheType* FindOrCreateArchetype(ArcheType* baseArcheType, ComponentID newComponentType) {
            Type components(baseArcheType->type);
            components.push_back(newComponentType);

            std::cout << "Adding Component: " << newComponentType << std::endl;
            for (int i = 0; i < components.size(); i++) {
                std::cout << components[i] << " ";
            }
            std::cout << std::endl;

            for (int i = 0; i < ArcheTypes.size(); i++) {
                if (compare(components, ArcheTypes[i]->type))
                    return ArcheTypes[i];
            }
            ArcheTypes.push_back(CreateArchetype(components));
            return ArcheTypes.back();
        }

        ArcheType* CreateArchetype(Type& components) {
            ArcheType* archeType = new ArcheType{
                components,
            };
            for (int i = 0; i < archeType->type.size(); i++)
                archeType->components.push_back(heapAlloc.allocate(TypeBlockSize));

            return archeType;
        }

        void moveComponentDataToArcheType(ArcheType* fromArcheType, ArcheType* toArcheType) {

        }


        template<typename T>
        void Set(EntityID& entityID, T type) {
            ComponentID newComponentType = Componentfamily::getID<T>();
            Record& rec = EntityIndex.at(entityID);
            ArcheType* previousArcheType = rec.archetype;
            ArcheType*& currentArcheType = rec.archetype;

            std::cout << "Current Components: "<< std::endl;
            for (int i = 0; i < currentArcheType->type.size(); i++) {
                std::cout << currentArcheType->type[i] << " ";
            }
            std::cout << std::endl;

            GraphEdge* TypeEdge = &currentArcheType->edges[newComponentType];
            if (!TypeEdge->add) {
                std::cout << "Needs to create ADD branch" << std::endl;
                TypeEdge->add = FindOrCreateArchetype(currentArcheType, newComponentType);
            }
            currentArcheType = TypeEdge->add;

            moveComponentDataToArcheType(previousArcheType, currentArcheType);
        }   

        template<typename T>
        ComponentID Component(const std::string& name) {
            ComponentID id = Componentfamily::getID<T>();
            //Root->edges.push_back()
            return id;
        }

        //void DeleteComponent(ComponentID id);
        //template<typename ...T> //, std::enable_if_t<(sizeof...(T) > 0)> * = nullptr
        //void CreateArcheType(){
        //    std::array<ComponentID, sizeof...(T)> test;
        //    size_t i = 0; 
        //    (void(test[i++] = Componentfamily::getID<T>()), ...);

        //    std::cout << "Types:";
        //    for (int i = 0; i < test.size(); i++) {
        //        std::cout << " " << test[i];
        //    }
        //    std::cout << std::endl;

        //    std::cout << "Archetype" << family::getID<T...>() << "\n";
        //}
        
        template<typename ...T>
        void System() {}



    private:
        EntityID    m_EntityIndex = 1;
        ComponentID m_ComponentIndex = 1;

        HeapAllocator<1024 * 1024 * 1024> heapAlloc;

        ArcheType* Root;
        Type AllTypes;
        std::unordered_map<EntityID, Record> EntityIndex;
        std::vector<ArcheType* > ArcheTypes;
    };
}