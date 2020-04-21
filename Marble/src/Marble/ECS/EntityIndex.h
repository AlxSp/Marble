#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>

namespace Marble {

    class Entity {

    typedef uint64_t ComponentId;
    typedef uint64_t EntityId;
    using Type = std::vector<ComponentId>;
    
    std::unordered_map<ComponentId, Type> ComponentIndex;

    ComponentId Position_id = 1;
    ComponentId Mass_id = 2;
    ComponentId Velocity_id = 3;

    //using Type = std::vector<EntityId>;
    
    std::unordered_map<EntityId, Type> EntityIndex;

    };

}
