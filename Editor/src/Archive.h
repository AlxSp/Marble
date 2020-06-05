#pragma once
#include <entt/entt.hpp>

class EnTTArchive {

    void operator()(entt::entity);

    void operator()(std::underlying_type_t<entt::entity>);

    //void operator()(entt::entity, const T &);
};

class Archive {

};