#ifndef GAMELIB_SCENE_OBJECT_HPP
#define GAMELIB_SCENE_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include "gamelib/core/geometry/Transformable.hpp"
#include "SceneData.hpp"
#include "Layer.hpp"

namespace gamelib
{
    class SceneObject : public SceneData,
                        public Transformable
    {
        public:
            SceneObject(int depth = 0, float parallax = 1, unsigned int flags = 0);
            virtual ~SceneObject() {}

            auto setLayer(Layer::Handle layer) -> void;
            auto getLayer() const              -> Layer::Handle;

            // Returns the flags merged with the layer's and scene's flags
            auto isVisible() const      -> bool;
            auto getAllFlags() const    -> unsigned int;
            auto getLocalBounds() const -> math::AABBf;
            auto getVertices() const    -> const sf::VertexArray&;
            auto size() const           -> size_t;
            auto getBBox() const        -> const math::AABBf&;

            auto unregister() -> void;

            auto render(sf::RenderTarget& target) const -> void;
            virtual auto render(sf::RenderTarget& target, sf::RenderStates states) const -> void;

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void;

            // Should be called by parents whenever the vertex list changes
            auto _updateBBox() -> void;

        protected:
            sf::VertexArray _vertices;

        private:
            Layer::Handle _layer;
            math::AABBf _bbox;
    };
}


#endif
