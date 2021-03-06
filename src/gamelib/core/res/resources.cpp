#include "gamelib/core/res/resources.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "gamelib/core/res/JsonResource.hpp"
#include "gamelib/core/res/SpriteResource.hpp"
#include "gamelib/core/res/EntityResource.hpp"
#include "gamelib/core/res/SoundResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"


namespace gamelib
{
    void registerPredefLoaders()
    {
        auto sys = getSubsystem<ResourceManager>();
        if (sys)
            registerPredefLoaders(*sys);
    }

    void registerPredefLoaders(ResourceManager& resmgr)
    {
        registerTextureLoader(resmgr);
        registerSpriteLoader(resmgr);
        registerJsonLoader(resmgr);
        registerEntityConfigLoader(resmgr);
        registerSoundLoader(resmgr);
    }
}
