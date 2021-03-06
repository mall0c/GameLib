#include "gamelib/editor/tools/SpriteTool.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/imgui/resources.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/utils/conversions.hpp"
#include "imgui.h"

namespace gamelib
{
    SpriteTool::SpriteTool()
    {
        _sprite = getSubsystem<ResourceManager>()->find<SpriteResource>();
    }

    void SpriteTool::onMousePressed()
    {
        if (_sprite)
        {
            auto& pos = EditorShared::getMouseSnapped();
            auto handle = createEntity("brush_sprite", pos.x, pos.y);
            handle->findByName<SpriteComponent>()->change(_sprite);
            EditorShared::getSelectTool().select(handle);
        }
    }

    void SpriteTool::drawGui()
    {
        inputResource("Sprite", &_sprite);
        if (ImGui::CollapsingHeader("Preview", ImGuiTreeNodeFlags_DefaultOpen))
            previewSprite(_sprite.asBase());
    }

    void SpriteTool::render(sf::RenderTarget& target)
    {
        if (_sprite && !InputSystem::getActive()->isMouseConsumed())
        {
            sf::Sprite spr(*_sprite->tex, convert(_sprite->rect));
            spr.setOrigin(convert(_sprite->origin.asVector()));
            spr.setPosition(convert(EditorShared::getMouseSnapped().asVector()));
            spr.setColor(sf::Color(255, 255, 255, 128));
            target.draw(spr);
        }
    }
}
