#ifndef EDITOR_VERTEXTOOL_HPP
#define EDITOR_VERTEXTOOL_HPP

#include "Tool.hpp"

namespace gamelib
{
    class VertexTool : public Tool
    {
        public:
            VertexTool();

            void onMousePressed() final override;
            void onDrag() final override;
            void drawGui() final override;
            void render(sf::RenderTarget& target) final override;

        private:
            size_t _selected;
            bool _snappoint;
    };
}

#endif
