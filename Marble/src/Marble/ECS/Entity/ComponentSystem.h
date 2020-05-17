#pragma once
#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/EntityManager.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"

namespace ECS {

    struct ArcheType;
    //struct MemBlk;

    template<typename ...T>
    constexpr std::array<ComponentID, sizeof...(T)> GetComponentTypesArr() noexcept {
        std::array<ComponentID, sizeof...(T)> componentIDs;
        size_t i = 0;
        (void(componentIDs[i++] = Componentfamily::getID<T>()), ...);
        return componentIDs;
    }

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

    class BaseComponentSystem
    {
    public:
        virtual ~BaseComponentSystem() = default;
        /*virtual bool Init() = 0;
        virtual void OnUpdate(float deltaTime) = 0;*/
    };

    template<class ...Components>
    class ComponentSystem : public BaseComponentSystem
    {
    public:
        ComponentSystem(std::vector<ArcheType*>& ArcheTypes);
        virtual ~ComponentSystem() override = default;
    private:
        template<size_t Index, typename ComponentType, typename ...RemainingComponentTypes>
        void AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, std::tuple<Components*...>& tuple);
        
        template<size_t Index>
        void AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, std::tuple<Components*...>& tuple);
        
    private:
        using ComponentTuple = std::tuple<Components*...>;
        std::vector<ComponentTuple> componentCollections;
        std::array<ComponentID, sizeof...(Components)> componentIDs = GetComponentTypesArr<Components...>();
        /*virtual bool Init() override;
        virtual void OnUpdate(float deltaTime) = 0;*/
    };

    template <typename... Components>
    ComponentSystem<Components...>::ComponentSystem(std::vector<ArcheType*>& ArcheTypes) : BaseComponentSystem() {
        std::array<uint16_t, sizeof...(Components)> componentIndices;
        for (int i = 0; i < ArcheTypes.size(); i++) {   //iterate over all created ArcheTypes 
            if (ArcheTypes[i]->length > 0) {
                Type& type = ArcheTypes[i]->type;
                if (hasComponentSet<sizeof...(Components)>(type, componentIDs, componentIndices)) {
                    std::cout << "Found Matching ArcheType" << std::endl;
                    ComponentTuple tuple;
                    AddComponentPtrsToTuple<0, Components...>(ArcheTypes[i]->components, componentIndices, tuple);
                    componentCollections.emplace_back(tuple);
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



   /* template<typename... Components>
    class Query {
        using ComponentTuple = std::tuple<Components*...>;

    };*/



    template<class ...Components>
    template<size_t Index, typename ComponentType, typename ...RemainingComponentTypes>
    inline void ComponentSystem<Components...>::AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, std::tuple<Components*...>& tuple)
    {
        std::get<Index>(tuple) = static_cast<ComponentType*>(archetypeComponents[componentIndices[Index]].ptr);
        AddComponentPtrsToTuple<Index + 1, RemainingComponentTypes...>(archetypeComponents, componentIndices, tuple);
    }

    template<class ...Components>
    template<size_t Index>
    inline void ComponentSystem<Components...>::AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, std::tuple<Components*...>& tuple) {
        return;
    }


}