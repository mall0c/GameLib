#include "gamelib/components/editor/LineBrushComponent.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    constexpr const char* LineBrushComponent::name;

    LineBrushComponent::LineBrushComponent() :
        _line(nullptr),
        _linewidth(32)
    {
        auto widthSetter = +[](int*, const int* val, LineBrushComponent* self) { self->setWidth(*val); };

        _props.registerProperty("linewidth", _linewidth, widthSetter, this);
    }

    void LineBrushComponent::setWidth(int width)
    {
        if (_linewidth == width)
            return;

        _linewidth = width;
        regenerate();
    }

    int LineBrushComponent::getWidth() const
    {
        return _linewidth;
    }

    Polygon* LineBrushComponent::getBrushPolygon() const
    {
        return _line;
    }

    void LineBrushComponent::_refresh()
    {
        BrushComponent::_refresh();

        _line = nullptr;
        getEntity()->findAllByName<Polygon>([&](Polygon* pol) {
                if (pol->getPolygon().type == math::LineStrip)
                {
                    if (_line != pol)
                    {
                        _line = pol;
                        regenerate();
                    }
                    return true;
                }
                return false;
            });

    }

    void LineBrushComponent::regenerate() const
    {
        if (!_line || _line->getPolygon().size() == 0)
            return;

        math::Vec2f lastd, dir;
        bool start = true;
        _pol->clear();

        auto cb = [&](const math::Line2f& seg) {
            float area = 0;
            if (start)
            {
                start = false;
                lastd = seg.d;
            }
            else
            {
                // Compute if the line turns clockwise or counter clockwise
                // Total hours wasted here because I didn't google the
                // problem / didn't knew what exactly to google for: 11
                auto p1 = seg.p - lastd;
                auto p3 = seg.p + seg.d;

                // Break if the direction hasn't changed
                if (lastd.normalized() == seg.d.normalized())
                    return false;

                area = (seg.p.x - p1.x) * (seg.p.y + p1.y)
                    + (p3.x - seg.p.x) * (p3.y + seg.p.y)
                    + (p1.x - p3.x) * (p1.y + p3.y);
            }

            dir = (seg.d.normalized() + lastd.normalized()).right().normalized();// / 2;
            dir *= (_linewidth / 2.0) / dir.angle_cos(lastd.right());

            _pol->add(seg.p - dir);
            _pol->add(seg.p + dir);

            // depending on clockwise or counter clockwise line direction,
            // different vertices have to be added.
            if (area != 0)
            {
                if (area > 0)   // clockwise
                {
                    _pol->add(seg.p - dir);
                    _pol->add(seg.p - dir + seg.d.normalized().right() * _linewidth);
                }
                else   // counter clockwise
                {
                    _pol->add(seg.p + dir - seg.d.normalized().right() * _linewidth);
                    _pol->add(seg.p + dir);
                }
            }

            lastd = seg.d;
            return false;
        };

        auto& linepol = _line->getPolygon();
        linepol.foreachSegment(cb);
        dir = lastd.right().normalized() * (_linewidth / 2.0);
        _pol->add(linepol.get(-1) - dir);
        _pol->add(linepol.get(-1) + dir);

        // BrushComponent::regenerate();
        if (_shape && _pol)
            _shape->fetch(_pol->getPolygon(), MapLine);
    }
}