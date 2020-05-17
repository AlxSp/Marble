#pragma once

#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/ComponentSystem.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"

#include <iostream>
#include <memory>
#include <algorithm>
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
            static uint64_t value = 0;
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

    //class ComponentObject {
    //    class Concept {
    //    public:
    //        virtual size_t getComponentSize() = 0;
    //    };

    //    template<typename T>
    //    class Model
    //};

    //class ComponentObject {
    //public:
    //    template<class T>
    //    ComponentObject(T obj) { 
    //        objectPtr = std::make_unique<Model<T>>(std::move(obj));
    //    }

    //    size_t getComponentSize() const {
    //        return objectPtr->getComponentSize();
    //    }

    //    void* getPointerAt(size_t index) const {
    //        return objectPtr->getPointerAt(index);
    //    }

    //    struct Concept {
    //        virtual const size_t getComponentSize() const = 0;
    //        virtual void* getPointerAt(size_t index) const = 0;
    //    };

    //    template<typename T>
    //    struct Model : public Concept
    //    {
    //        explicit Model(T&& componentData) : ComponentData(std::move(componentData)) {}

    //        const size_t getComponentSize() const override {
    //            return ComponentData.getComponentSize();
    //        }

    //        void* getPointerAt(size_t index) const override {
    //            return ComponentData.getPointerAt(index);
    //        }

    //    private:
    //        T ComponentData;
    //    };

    //    std::unique_ptr<Concept> objectPtr;
    //};


    /*struct ComponentArray {
        void* elements;
        size_t size;
    };*/




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
            EntityIndex.insert({ m_EntityIndex, {Root, 0} }); //Create new Entity with the Root as inital ArcheType and 0 as index
            return m_EntityIndex++; //Increment Entity Index 
        }

        ArcheType* FindOrCreateArchetype(ArcheType* baseArcheType, ComponentID newComponentType) {
            Type type(baseArcheType->type);     //Create new type for the ArcheType
            type.push_back(newComponentType);   //Add new Component Type to type

            /*std::cout << "Adding Component: " << newComponentType << std::endl;
            for (int i = 0; i < type.size(); i++) {
                std::cout << type[i] << " ";
            }
            std::cout << std::endl;*/
            for (int i = 0; i < ArcheTypes.size(); i++) { // Go through exisitng ArcheTypes to check if ArcheType already exists
                if (compare(type, ArcheTypes[i]->type))   // Compare type with ArcheType type 
                    return ArcheTypes[i];                 // if ArcheTypes match return already existing ArcheType pointer
            }
            ArcheTypes.push_back(CreateArchetype(type));  // else create new ArcheType with type and add to ArcheType vector
            return ArcheTypes.back();   //return new ArcheType pointer
        }

        ArcheType* CreateArchetype(Type& type) {
            ArcheType* archeType = new ArcheType{       // Create new ArcheType with type
                type,
            };
            for (int i = 0; i < archeType->type.size(); i++)    //Allocate Memory from heap for each type
                archeType->components.push_back(heapAlloc.allocate(TypeBlockSize));

            return archeType; //return pointer of new ArcheType
        }

        int findIndexOfComponent(const ComponentID& id, const Type& type) {
            for (int i = 0; i < type.size(); i++) { //Go through Type
                if (type[i] == id) return i;        //return index of Component in Type
            }
            return -1;  // else return -1 !!!BAD!!!
        }

        //Move Entity to new ArcheType when new Component is added
        template<typename T>
        void moveEntityData(Record& entityRecord, ArcheType* toArcheType, T& newData) { 
            int index = findIndexOfComponent(Componentfamily::getID<T>(), toArcheType->type); //Get index of new Component in destination ArcheType
            T* newComponentArrPtr = static_cast<T*>(toArcheType->components[index].ptr);      //Get destination pointer of new Component
            newComponentArrPtr[toArcheType->length] = newData;  //Add new Component to Component Array

            for (int i = 0; i < entityRecord.archetype->type.size(); i++) { //Copy remaining Components from source ArcheType to destination ArcheType
                size_t componentSize = ComponentSizes[entityRecord.archetype->type[i]]; //Get Component byte size
                int componentIndex = findIndexOfComponent(entityRecord.archetype->type[i], toArcheType->type); //Get index of Component in destination ArcheType
                void* fromPtr = static_cast<char*>(entityRecord.archetype->components[i].ptr) + entityRecord.rowIndex * componentSize;  //Get pointer to component in source ArcheType
                void* toPtr = static_cast<char*>(toArcheType->components[componentIndex].ptr) + toArcheType->length * componentSize; //Get pointer for component in destination ArcheType
                std::memcpy(toPtr, fromPtr, componentSize); //Copy component Data
            }
            //Ugly code !!!
            entityRecord.rowIndex = toArcheType->length; //Set Entity index to length of destination ArcheType (end of array index)  
            //decrease length of source ArcheType !!! Should check if Component was at end of array. If not move follow elements
            entityRecord.archetype->length--;  
            entityRecord.archetype = toArcheType; //Set Entity ArcheType pointer to destination ArcheType
            entityRecord.archetype->length++; //increase length of destination/Entity ArcheType by 1
        }

        //Add Component to the Entity
        template<typename T>
        void Add(EntityID& entityID, T type) {
            std::cout << "\n";
            ComponentID componentID = Componentfamily::getID<T>();  //Get ID for new Component 
            Record& rec = EntityIndex.at(entityID);                 //Get Info of Entity 
            ArcheType* currentArcheType = rec.archetype;            //Get pointer to Entity ArcheType

            GraphEdge* TypeEdge = &currentArcheType->edges[componentID]; //Get Edge to new Component 
            if (!TypeEdge->add) {   // if edge does not exist 
                std::cout << "Needs to create ADD branch" << std::endl;
                TypeEdge->add = FindOrCreateArchetype(currentArcheType, componentID);   //find or create destination ArcheType and set it as new Edge
            }
            currentArcheType = TypeEdge->add; //make destination ArcheType the current ArcheType


            std::cout << "Current Components: " << std::endl;
            for (int i = 0; i < currentArcheType->type.size(); i++) {
                std::cout << currentArcheType->type[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "Current ArcheType Length: " << currentArcheType->length << std::endl;

            moveEntityData(rec, currentArcheType, type);    //Move Entity Component Data to destination ArcheType
        }   

        template<typename T>
        T* GetPtr(EntityID& entityID) { //Get Component pointer of Entity ID
            Record& rec = EntityIndex.at(entityID); //Get Entity Record reference  
            int index = findIndexOfComponent(Componentfamily::getID<T>(), rec.archetype->type); //Get Component Index in the Entity's ArcheType  
            return static_cast<T*>(rec.archetype->components[index].ptr) + rec.rowIndex; //Return pointer to Component of Entity
        }

        //Is this function really necessary?
        template<typename T>
        T& GetRef(EntityID& entityID) { //Get Component reference of Entity ID
            Record& rec = EntityIndex.at(entityID); //Get Entity Record reference  
            int index = findIndexOfComponent(Componentfamily::getID<T>(), rec.archetype->type); //Get Component Index in the Entity's ArcheType  
            return *(static_cast<T*>(rec.archetype->components[index].ptr) + rec.rowIndex); //Return reference to Component of Entity
        }

        template<typename T>
        ComponentID AddComponent() {

            //std::vector<ComponentObject> vec{ ComponentObject(Test()) }; // Object(Foo()), Object(Bar()) 
            //Component<T> object(heapAlloc.allocate(256));
            //ComponentObject obj(object);
            //ComponentObject obj = ComponentObject(new Component<T>(heapAlloc.allocate(256)));
            ComponentID id = Componentfamily::getID<T>();
            ComponentSizes.push_back(sizeof(T));
            return id;
        }

        template<typename ...T>
        constexpr std::array<ComponentID, sizeof...(T)> GetComponentTypesArr() noexcept {
            std::array<ComponentID, sizeof...(T)> componentIDs;
            size_t i = 0;
            (void(componentIDs[i++] = Componentfamily::getID<T>()), ...);
            return componentIDs;
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

        //struct
        //Types are not sorted yet
        template<size_t componentSetSize>
        bool hasComponentSet(Type& sortedType, std::array<ComponentID, componentSetSize>& sortedComponentSet, std::array<uint16_t, componentSetSize>& componentIndices) {
            uint16_t foundComponents = 0;
            for (int i = 0; i < sortedType.size(); i++) {
                if (sortedComponentSet[foundComponents] == sortedType[i])
                    componentIndices[foundComponents] = i;
                    foundComponents++;
                if (foundComponents == componentSetSize)
                    return true;
            }
            return false;
        }

        //template<class ...Components>
        //template<size_t Index, typename ComponentType, typename ...RemainingComponents>
        //void AddComponentPtrsToTuple(Type& type, std::tuple<Components*...>& tuple) {

        //}

        //template<size_t Index, typename ...T>
        //void AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(T)>& componentIndices, std::tuple<T*...>& tuple) {

        //}

        template<typename ...T>
        void CreateComponentSystem(std::vector<ArcheType*>& ArcheTypes) {
            using ComponentTuple = std::tuple<T*...>;
            std::array<ComponentID, sizeof...(T)> componentIDs = GetComponentTypesArr<T...>();
            std::array<uint16_t, sizeof...(T)> componentIndices;
            std::vector<ComponentTuple> componentCollections;
            for (int i = 0; i < ArcheTypes.size(); i++) {   //iterate over all created ArcheTypes 
                if (ArcheTypes[i]->length > 0) {
                    
                    Type& type = ArcheTypes[i]->type;
                    if (hasComponentSet<sizeof...(T)>(type, componentIDs, componentIndices)) {
                        std::cout << "Found Matching ArcheType" << std::endl;
                        ComponentTuple tuple;
                        AddComponentPtrsToTuple<0, T...>
                        //for (int j = 0; j < sizeof...(T); j++) {
                        //    //std::get<j>(tuple) = 
                        //}
                        componentCollections.push_back(tuple);
                        //AddComponentPtrsToTuple<0, T...>(type, componentCollections.back());
                    }
                    else {
                        std::cout << "No Match" << std::endl;
                    }
                    /*if ((std::find(type.begin(), type.end(), Componentfamily::getID<T>()) != type.end()) && ...) {
                        std::cout << "Found Matching ArcheType" << std::endl;
                    }*/
                }
            }
        }
        
        template<typename ...T>
        void System() {
            EntityID systemID = family::getID<T...>();
            if (systemID < Systems.size()) {
                //std::cout << "Found exisiting Component Collection " << std::endl;
                //return Systems[systemID];
            }
            else  {
                std::cout << "New Component Collection ID: " << systemID << std::endl;
                Systems.push_back(new ComponentSystem<T...>(ArcheTypes));
                //CreateComponentSystem<T...>(ArcheTypes);
            }
            
            //return new ComponentSystem<T...>();
            //using ComponentSetTuple = std::tuple<T*...>;
            //std::array<ComponentID, sizeof...(T)> componentIDs = GetComponentTypesArr<T...>();
            //std::vector<ComponentSetTuple> components;
            //for (int i = 0; i < ArcheTypes.size(); i++) {   //iterate over all created ArcheTypes 
            //    if (ArcheTypes[i]->length > 0) {
            //        Type& type = ArcheTypes[i]->type;
            //        if (hasComponentSet<sizeof...(T)>(type, componentIDs)) {
            //            std::cout << "Found Matching ArcheType" << std::endl;
            //        }
            //        else {
            //            std::cout << "No Match" << std::endl;
            //        }
            //        /*if ((std::find(type.begin(), type.end(), Componentfamily::getID<T>()) != type.end()) && ...) {
            //            std::cout << "Found Matching ArcheType" << std::endl;
            //        }*/
            //    }
            //}
        }



    private:
        EntityID    m_EntityIndex = 1;
        ComponentID m_ComponentIndex = 1;

        HeapAllocator<1024 * 1024 * 1024> heapAlloc;

        ArcheType* Root;

        std::vector<size_t> ComponentSizes{0};
        std::vector<BaseComponentSystem*> Systems;
        std::unordered_map<EntityID, Record> EntityIndex;
        std::vector<ArcheType*> ArcheTypes;
    };
}