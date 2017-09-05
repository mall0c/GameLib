#ifndef GAMELIB_AABB_COLLISON_HPP
#define GAMELIB_AABB_COLLISON_HPP

#include "gamelib/ecs/CollisionComponent.hpp"

namespace gamelib
{
    class AABBCollision : public CollisionComponent
    {
        public:
            AABBCollision(float x, float y, float w, float h, unsigned int flags = 0);
            AABBCollision(const math::AABBf& aabb, unsigned int flags = 0);
            virtual ~AABBCollision() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto move(const math::Vec2f& rel)    -> void;
            auto scale(const math::Vec2f& scale) -> void;
            auto getPosition() const             -> const math::Point2f&;
            auto getScale() const                -> const math::Vec2f&;

            auto getBBox() const -> const math::AABBf&;

        protected:
            math::Vec2f _scale;
            math::AABBf _rect;
    };
}

#endif