#include "gamelib/res/EntityConfigResource.hpp"
#include "gamelib/ecs/EntityFactory.hpp"
#include "gamelib/res/ResourceManager.hpp"

namespace gamelib
{
    void registerEntityConfigLoader(ResourceManager& resmgr)
    {
        for (auto& i : { "ent", "entity" })
            resmgr.registerFileType(i, entityConfigLoader);
    }

    BaseResourceHandle entityConfigLoader(const std::string& fname, ResourceManager* resmgr)
    {
        auto res = jsonLoader(fname, resmgr);

        auto factory = getSubsystem<EntityFactory>();
        if (factory)
            factory->add(res.as<JsonResource>());

        return res;
    }
}