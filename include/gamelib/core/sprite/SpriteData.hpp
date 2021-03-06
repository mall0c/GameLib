#ifndef GAMELIB_SPRITE_DATA_HPP
#define GAMELIB_SPRITE_DATA_HPP

#include <SFML/Graphics/Rect.hpp>
#include "gamelib/core/res/TextureResource.hpp"
#include "math/geometry/AABB.hpp"

namespace gamelib
{
    auto getSpriteRect(int offset, const sf::IntRect& rect, int texw, int texh) -> sf::IntRect;
    auto getSpriteRect(int offset, const math::AABBi& rect, int texw, int texh) -> math::AABBi;

    class AnimationData
    {
        public:
            AnimationData();
            auto update(float elapsed) -> void;
            auto randomize()           -> void;
            auto setIndex(int index)   -> void;

        public:
            int length;
            float speed;
            float offset;
    };

    // More memory efficient than storing sf::Sprite or gamelib::AnimatedSprite
    // if a drawable sprite object isn't needed.
    class SpriteData
    {
        public:
            auto getRect(int texw, int texh) const -> sf::IntRect;

        public:
            AnimationData anidata;
            sf::Vector2f origin;
            sf::IntRect rect;
    };
}

#endif
