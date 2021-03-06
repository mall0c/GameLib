#ifndef GAMELIB_ENTITYFACTORY_HPP
#define GAMELIB_ENTITYFACTORY_HPP

#include "gamelib/utils/Factory.hpp"
#include "gamelib/core/res/EntityResource.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Component.hpp"
#include "Entity.hpp"

namespace gamelib
{
    auto createEntity(const std::string& name)                   -> EntityReference;
    auto createEntity(const std::string& name, float x, float y) -> EntityReference;


    class EntityFactory : public Subsystem<EntityFactory>
    {
        typedef Factory<Component, std::string> ComponentFactory;

        public:
            ASSIGN_NAMETAG("EntityFactory");

        public:
            EntityFactory();

            auto createWithDelta(const Json::Value& node)              -> EntityReference;
            auto createWithDelta(const Json::Value& node, Entity* ent) -> bool;
            auto createFromJson(const Json::Value& node)               -> EntityReference;
            auto createFromJson(const Json::Value& node, Entity* ent)  -> bool;
            auto create(const std::string& name)                       -> EntityReference;
            auto create(const std::string& name, Entity* ent)          -> bool;

            auto createComponent(const std::string& name)                                  -> ComponentPtr;
            auto createComponentFromJson(const std::string& name, const Json::Value& node) -> ComponentPtr;

            auto add(EntityResource::Handle entcfg) -> void;
            auto addComponent(const std::string& name, ComponentFactory::CreatorFunction callback) -> void;
            auto removeEntity(const std::string& name)    -> void;
            auto removeComponent(const std::string& name) -> void;
            auto clear()       -> void;
            auto size() const  -> size_t;
            auto findEntity(const std::string& name) const -> EntityResource::Handle;

            template <typename T>
            void addComponent(const std::string& name)
            {
                _compfactory.add<T>(name);
                LOG_DEBUG("Registered component ", name);
            }

            template <typename T, typename std::enable_if<has_nametag<T>(), int>::type = 0>
            void addComponent()
            {
                addComponent<T>(T::name());
            }

            // Signature: bool(const std::string&)
            // When true is returned, the loop breaks.
            template <typename F>
            void iterkeys(F callback) const
            {
                for (auto& i : _entdata)
                    if (callback(i.first))
                        break;
            }

        private:
            auto _findTemplate(const std::string& name) -> EntityResource::Handle;

        private:
            std::unordered_map<std::string, EntityResource::Handle> _entdata;
            ComponentFactory _compfactory;
    };
}

#endif
