#ifndef GAMELIB_PROPERTY_BASIC_TYPES_HPP
#define GAMELIB_PROPERTY_BASIC_TYPES_HPP

#include "PropType.hpp"
#include "gamelib/core/Identifier.hpp"
#include "gamelib/utils/json.hpp"
#include "math/geometry/Vector.hpp"
#include "gamelib/core/res/Resource.hpp"
#include <SFML/Graphics/Color.hpp>

namespace gamelib
{
    template <typename T>
    class BasicTypeSerializer : public BasePropType<T>
    {
        public:
            virtual ~BasicTypeSerializer() {};

            bool loadFromJson(const PropertyHandle& prop, T* ptr, const Json::Value& node) const
            {
                loadFromJsonBasic(node, *ptr);
                return true;
            }

            void writeToJson(const PropertyHandle& prop, Json::Value& node) const
            {
                node = prop.getAs<T>();
            }
    };

    template <typename T>
    class VectorSerializer : public BasePropType<T>
    {
        public:
            virtual ~VectorSerializer() {};

            bool loadFromJson(const PropertyHandle& prop, T* ptr, const Json::Value& node) const
            {
                gamelib::loadFromJson(node, *ptr);
                return true;
            }

            void writeToJson(const PropertyHandle& prop, Json::Value& node) const
            {
                gamelib::writeToJson(node, prop.getAs<T>());
            }
    };

    class PropResource : public PropType<0xd41af3a9, BaseResourceHandle>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, BaseResourceHandle* ptr, const Json::Value& node) const;
            void writeToJson(const PropertyHandle& prop, Json::Value& node) const;
            bool drawGui(const PropertyHandle& prop, const std::string& name, BaseResourceHandle* ptr) const;
    };

    extern PropResource propResource;

    class PropColor : public PropType<0xf261806f, sf::Color>
    {
        public:
            bool loadFromJson(const PropertyHandle& prop, sf::Color* ptr, const Json::Value& node) const;
            void writeToJson(const PropertyHandle& prop, Json::Value& node) const;
            bool drawGui(const PropertyHandle& prop, const std::string& name, sf::Color* ptr) const;
    };

    extern PropColor propColor;


#define GENERATE_CATEGORIZER(name, type)  \
    inline const IPropType* categorizeProperty(const type&)   \
    {   \
        return &prop##name; \
    }

#define GENERATE_PROP(name, id, type, base) \
    class Prop##name : public Identifier<id, base<type>> \
    {   \
        public: \
            auto drawGui(const PropertyHandle& prop, const std::string& name, type* ptr) const -> bool; \
    };  \
    extern Prop##name prop##name;   \
    GENERATE_CATEGORIZER(name, type)

#define GENERATE_BASIC_PROP(name, id, type) GENERATE_PROP(name, id, type, BasicTypeSerializer)
#define GENERATE_VECTOR_PROP(name, id, type) GENERATE_PROP(name, id, type, VectorSerializer)


    GENERATE_BASIC_PROP(Int, 0x0b31b24c, int);
    GENERATE_BASIC_PROP(Bitflags, 0xfe5aeea2, unsigned int);
    GENERATE_BASIC_PROP(Float, 0x2f178d03, float);
    GENERATE_BASIC_PROP(Bool, 0x4d746867, bool);
    GENERATE_BASIC_PROP(String, 0x8e7ae0b1, std::string);

    GENERATE_VECTOR_PROP(Vec2f, 0xf4598369, math::Vec2f);
    GENERATE_VECTOR_PROP(Vec2i, 0xf55f0d17, math::Vec2i);

    GENERATE_CATEGORIZER(Resource, BaseResourceHandle)
    GENERATE_CATEGORIZER(Color, sf::Color)

#undef GENERATE_CATEGORIZER
#undef GENERATE_BASIC_PROP
#undef GENERATE_VECTOR_PROP
#undef GENERATE_PROP


    template <typename T>
    const IPropType* categorizeProperty(const T&)
    {
        static_assert(std::is_enum<T>(), "Type could not be auto-detected");
        return &propInt;
    }

    template <typename T>
    const IPropType* categorizeProperty(const ResourceHandle<T>&)
    {
        return &propResource;
    }
}

#endif