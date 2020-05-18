#pragma once
#include "Marble/ECS/Entity/EntityDefinitions.h"
#include "Marble/ECS/Entity/EntityManager.h"
#include "Marble/ECS/Memory/MemoryAllocator.h"

#include <functional>

namespace ECS {

    struct ArcheType;

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

    class BaseComponentSystem {};

    template<class ...Components>
    class ComponentSystem : public BaseComponentSystem
    {
    public:
        ComponentSystem(std::vector<ArcheType*>& ArcheTypes);
        //virtual ~ComponentSystem() override = default;
        template <typename Functor>
        void each(Functor functor);

    private:
        using ComponentTuple = std::tuple<Components*...>;
        using ComponentInfoTuple = std::tuple<ComponentTuple, size_t>;

        //template <size_t N, >

        /*template<Index>
        void IncrementTuplePtrs(ComponentTuple& tuple, uint16_t incrementValue);*/

        template<size_t Index, typename ComponentType, typename ...RemainingComponentTypes>
        void AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, ComponentTuple& tuple);
        
        template<size_t Index>
        void AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, ComponentTuple& tuple);
        
    private:
        std::vector<ComponentInfoTuple> componentCollections;
        std::array<ComponentID, sizeof...(Components)> componentIDs = GetComponentTypesArr<Components...>();
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
                    componentCollections.emplace_back(tuple, ArcheTypes[i]->length);
                }
                else {
                    std::cout << "No Match" << std::endl;
                }
            }
        }
    }

    /*template<class ...Components>
    template<tyIndex>
    void IncrementTuplePtrs(ComponentTuple& tuple, uint16_t incrementValue);*/

    template<class ...Components>
    template<typename Func>
    inline void ComponentSystem<Components...>::each(Func func)
    {
        for (auto& componentInfo : componentCollections) {
            auto [componentPtrTuple, componentCount] = componentInfo;
            
            for (int i = 0; i < componentCount; i++) {
                std::apply(func, componentPtrTuple);
                std::apply([](auto& ...ptr) {((ptr += 1), ...);}, componentPtrTuple);
            }

        }
    }

    template<class ...Components>
    template<size_t Index, typename ComponentType, typename ...RemainingComponentTypes>
    inline void ComponentSystem<Components...>::AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, ComponentTuple& tuple)
    {
        std::get<Index>(tuple) = static_cast<ComponentType*>(archetypeComponents[componentIndices[Index]].ptr);
        AddComponentPtrsToTuple<Index + 1, RemainingComponentTypes...>(archetypeComponents, componentIndices, tuple);
    }

    template<class ...Components>
    template<size_t Index>
    inline void ComponentSystem<Components...>::AddComponentPtrsToTuple(std::vector<MemBlk>& archetypeComponents, std::array<uint16_t, sizeof...(Components)>& componentIndices, ComponentTuple& tuple) {
        return;
    }


}