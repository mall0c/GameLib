#include "gamelib/ecs/Entity.hpp"
#include "gamelib/ecs/EntityFactory.hpp"
#include <cassert>

using namespace gamelib;

class ComponentA : public Identifier<0x76d44c07, Component>
{
    public:
        ComponentA() : Identifier("ComponentA") {}
};

class ComponentB : public Identifier<0x650e189e, Component>
{
    public:
        ComponentB() : Identifier("ComponentB") {}

        bool loadFromJson(const Json::Value& node)
        {
            x = node.get("x", 0).asInt();
            return Component::loadFromJson(node);
        }

    public:
        int x;
};

void testEntity(Entity& entity)
{
    assert(entity.getTransform().getPosition() == math::Point2f(5, 5));
    assert(entity.getTransform().getScale() == math::Vec2f(-1.5, -1.5));
    assert(entity.getTransform().getRotation() == 90);
    assert(entity.size() == 2);
    assert(entity.find<ComponentA>());
    assert(entity.find<ComponentB>());
    assert(entity.find<ComponentB>()->x == 42);
}


int main(int argc, char *argv[])
{
    auto res = JsonResource::create();
    Json::Reader().parse("{\n\
            \"name\": \"testentity\",\n\
            \"transform\": {\n\
                \"pos\": {\n\
                    \"x\": 5,\n\
                    \"y\": 5\n\
                },\n\
                \"scale\": {\n\
                    \"x\": -1.5,\n\
                    \"y\": -1.5\n\
                },\n\
                \"angle\": 90\n\
            },\n\
            \"components\": [\n\
                {\n\
                    \"name\": \"ComponentA\"\n\
                },\n\
                {\n\
                    \"name\": \"ComponentB\",\n\
                    \"x\": 42\n\
                }\n\
            ]\n\
        }", *res);

    EntityFactory factory;
    factory.addComponent<ComponentA>("ComponentA");
    factory.addComponent<ComponentB>("ComponentB");
    factory.add(res);

    Entity entity;
    factory.create("testentity", &entity);
    testEntity(entity);

    factory.removeEntity("testentity");

    entity.destroy();
    factory.createFromJson(*res, &entity);
    testEntity(entity);

    return 0;
}