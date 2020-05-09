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

    class Componentfamily {
    public:
        template<typename T>
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

    template<typename T>
    struct ComponentMemory {
        T* elements;
        size_t size;
    };

    template<typename T>
    struct Component {
        Component(MemBlk memory) : Memory(memory) { 
            CompMem.elements = static_cast<T*>(memory.ptr); 
            CompMem.size = memory.size / sizeof(T);
        }

    private:
        size_t ComponentSize = sizeof(T);
        ComponentMemory<T> CompMem;
        MemBlk Memory;
    };

    class ComponentObject {
        struct Concept {
            virtual const size_t getComponentSize() const = 0;
            virtual const void* getPointerAt(size_t index) const = 0;
        };

        template<typename T>
        struct Model : public Concept
        {
            Model(const T& componentData) : ComponentData(componentData) {}

            const size_t getComponentSize() const override {
                return ComponentData.getComponentSize();
            }

            const void* getPointerAt(size_t index) const override {
                return ComponentData.getComponentSize(index);
            }

        private:
            const T ComponentData;
        };
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

    struct Record {
        ArcheType* archetype;
        int rowIndex;
    };

    template<typename T>
    bool compare(std::vector<T>& v1, std::vector<T>& v2) {
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        return v1 == v2;
    }

    class EntityManager 
    {
    public:
        EntityManager() { Root = new ArcheType(); }
        EntityManager(const EntityManager&) { delete Root; }

        EntityID Entity() {
            EntityIndex.insert({ m_EntityIndex, {Root, 0} });
            return m_EntityIndex++;
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

        int findIndexOfComponent(ComponentID id, Type& type) {
            for (int i = 0; i < type.size(); i++) {
                if (type[i] == id) return i;
            }
            return -1;
        }

        template<typename T>
        void moveEntityData(Record& entityRecord, ArcheType* toArcheType, T& newData) {
            int index = findIndexOfComponent(Componentfamily::getID<T>(), toArcheType->type);

            T* newComponentArrPtr = static_cast<T*>(toArcheType->components[index].ptr);
            newComponentArrPtr[toArcheType->length] = newData;

            for (int i = 0; i < entityRecord.archetype->type.size(); i++) {
                int componentIndex = findIndexOfComponent(entityRecord.archetype->type[i], toArcheType->type);
                //static_cast<T*>(entityRecord.archetype->components[i].ptr)[entityRecord.rowIndex] = ;
            }

            entityRecord.rowIndex = toArcheType->length;
            entityRecord.archetype->length--;
            entityRecord.archetype = toArcheType;
            entityRecord.archetype->length++;
        }


        template<typename T>
        void Add(EntityID& entityID, T type) {
            std::cout << "\n";
            ComponentID newComponentType = Componentfamily::getID<T>();
            Record& rec = EntityIndex.at(entityID);
            ArcheType* currentArcheType = rec.archetype;

            GraphEdge* TypeEdge = &currentArcheType->edges[newComponentType];
            if (!TypeEdge->add) {
                std::cout << "Needs to create ADD branch" << std::endl;
                TypeEdge->add = FindOrCreateArchetype(currentArcheType, newComponentType);
            }
            currentArcheType = TypeEdge->add;


            std::cout << "Current Components: " << std::endl;
            for (int i = 0; i < currentArcheType->type.size(); i++) {
                std::cout << currentArcheType->type[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "Current ArcheType Length: " << currentArcheType->length << std::endl;


            moveEntityData(rec, currentArcheType, type);

        }   

        template<typename T>
        ComponentID Component(const std::string& name) {
            ComponentID id = Componentfamily::getID<T>();
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
        std::vector<ArcheType*> ArcheTypes;
    };
}