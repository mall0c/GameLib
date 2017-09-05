#include "gamelib/movement/TileWalker.hpp"

namespace gamelib
{
    TileWalker::TileWalker(float x, float y, float w, float h, const BaseTilemapCollider& c) :
        AABBCollision(x, y, w, h),
        _col(&c)
    { }

    void TileWalker::moveContact(float dx, float dy)
    {
        if (dx)
            _moveContact(dx, 0);
        if (dy)
            _moveContact(dy, 1);
    }

    bool TileWalker::placeFree(float x, float y) const
    {
        return !_col->intersect(math::AABBf(x, y, _rect.size.x, _rect.size.y));
    }

    bool TileWalker::placeFreeRelative(float x, float y) const
    {
        return !_col->intersect(math::AABBf(_rect.pos.x + x, _rect.pos.y + y,
                    _rect.size.x, _rect.size.y));
    }

    void TileWalker::_moveContact(float val, int index)
    {
        // Move first, then move outside
        _rect.pos[index] += val;

        // Move outside
        if (_col->intersect(_rect))
        {
            auto tsize = _col->getTileSize();

            // Calculate the amount of pixels the object has moved into the current tile.
            float a;
            if (val < 0)
            {
                a = tsize[index] - math::sign(_rect.pos[index]) * std::fmod(_rect.pos[index], tsize[index]);
            }
            else if (val > 0)
            {
                if (_rect.pos[index] < 0)
                    a = tsize[index] + std::fmod(_rect.pos[index] + _rect.size[index], tsize[index]);
                else
                    a = std::fmod(_rect.pos[index] + _rect.size[index], tsize[index]);
            }

            _rect.pos[index] -= math::sign(val) * a;

            while (_col->intersect(_rect))
                _rect.pos[index] -= math::sign(val) * tsize[index];
        }
    }
}