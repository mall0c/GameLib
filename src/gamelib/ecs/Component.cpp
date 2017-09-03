#include "gamelib/ecs/Component.hpp"
#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/System.hpp"

namespace gamelib
{
    Component::Component() :
        _entptr(nullptr)
    { }

    Component::Component(const std::string& name) :
        _name(name),
        _entptr(nullptr)
    { }

    const std::string& Component::getName() const
    {
        return _name;
    }

    Entity* Component::getEntity() const
    {
        if (_entptr || _ent.isNull())
            return _entptr;
        return System::getActive()->getEntityManager()->get(_ent);
    }

    Component::Handle Component::getEntityHandle() const
    {
        return _ent;
    }

    bool Component::loadFromJson(const Json::Value& node)
    {
        return true;
    }

    void Component::writeToJson(Json::Value& node)
    {
        node["name"] = getName();
    }

    bool Component::_init()
    {
        return true;
    }

    void Component::_quit()
    { }

    void Component::_refresh()
    { }
}
