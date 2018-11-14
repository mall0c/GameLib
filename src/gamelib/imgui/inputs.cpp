#include "gamelib/imgui/inputs.hpp"
#include "gamelib/imgui/resources.hpp"
#include "gamelib/properties/Property.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "imgui.h"

namespace gamelib
{
    bool inputBitflags(const char* label, unsigned int* flags, int num, const char* const* names)
    {
        bool changed = false;
        ImGui::Text("%s", label);
        ImGui::Indent(1);
        for (int i = 0; i < num; ++i)
            if (names[i])
                if (ImGui::CheckboxFlags(names[i], flags, 1 << i))
                    changed = true;
        ImGui::Unindent(1);
        return changed;
    }


    bool inputLayer(const char* label, Layer::Handle* handle)
    {
        int selected = 0;
        auto scene = Scene::getActive();
        std::vector<Layer::Handle> cache;

        int i = 1;
        cache.push_back(Layer::Handle());
        scene->foreachLayer([&](Layer::Handle h, Layer&) {
                if (handle && h == *handle)
                    selected = i;
                cache.push_back(h);
                ++i;
            });

        auto itemgetter = [](void* cache_, int index, const char** name) {
            auto& handles = *static_cast<decltype(cache)*>(cache_);
            if (handles[index].isNull())
                *name = "default";
            else
                *name = Scene::getActive()->getLayer(handles[index])->getName().c_str();
            return true;
        };

        if (ImGui::Combo(label, &selected, itemgetter, (void*)&cache, cache.size()))
        {
            *handle = cache[selected];
            return true;
        }

        return false;
    }


    void inputSceneData(SceneData& sd)
    {
        int depth = sd.getDepth();
        float parallax = sd.getParallax();

        inputBitflags("Render flags", &sd.flags, num_renderflags, str_renderflags);

        if (ImGui::InputInt("Depth", &depth, 1, 100))
            sd.setDepth(depth);

        if (ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3))
            sd.setParallax(parallax);
    }

    void inputRenderComponent(RenderComponent& ren)
    {
        inputSceneData(ren);

        Layer::Handle handle = ren.getLayer();
        if (inputLayer("Layer", &handle))
            ren.setLayer(handle);
    }

    bool inputTransform(Transformable& trans)
    {
        // TODO: switch between global and local

        TransformData data = trans.getLocalTransformation();
        bool changed = false;

        if (trans.isMovable())
            if (ImGui::InputFloat2("Position", &data.pos[0], 2))
                changed = true;

        if (trans.isScalable())
            if (ImGui::InputFloat2("Scale", &data.scale[0], 2))
                changed = true;

        if (trans.isRotatable())
            if (ImGui::InputFloat("Rotation", &data.angle, 1, 10, 2))
                changed = true;

        if (ImGui::InputFloat2("Origin", &data.origin[0], 2))
            changed = true;

        if (changed)
            trans.setLocalTransformation(data);

        return changed;
    }


    bool inputProperties(const PropertyContainer& props)
    {
        bool changed = false;
        for (auto it : props)
            if (inputProperty(it.first, it.second))
                changed = true;
        return changed;
    }

    bool inputProperty(const std::string& name, const PropertyHandle& handle)
    {
        if (handle.serializer)
            return handle.serializer->drawGui(handle, name);
        return false;
    }


    void inputEntityProps(Entity& ent)
    {
        // static constexpr const char* entityFlags[] { "Persistent (Unimplemented)", "Static (Unimplemented)" };

        ImGui::InputText("name", const_cast<char*>(ent.getName().c_str()), ent.getName().size(), ImGuiInputTextFlags_ReadOnly);
        // ImGui::Text("%s", ent.getName().c_str());
        ImGui::NewLine();

        if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen))
            inputTransform(ent.getTransform());

        // if (ImGui::CollapsingHeader("Flags"))
        //     inputBitflags(&ent.flags, 2, entityFlags);

        if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // static int index = 0;
            // static constexpr const char* comps[] { "ComponentA", "ComponentB", "ComponentC" };
            // ImGui::Combo("##newcomp", &index, comps, 3);
            // ImGui::SameLine();
            // ImGui::Button("Add");

            ImGui::PushItemWidth(0.5 * ImGui::GetWindowContentRegionWidth());
            ent.foreach([](Component* comp) {
                ImGui::PushID(comp);
                if (ImGui::TreeNode(comp->getName().c_str()))
                {
                    inputComponent(*comp);
                    ImGui::TreePop();
                }
                ImGui::PopID();
                return true;
            });
            ImGui::PopItemWidth();
        }
    }

    void inputComponent(Component& comp)
    {
        if (comp.getTransform())
            inputTransform(*comp.getTransform());

        if (comp.getID() == RenderComponent::id)
            inputRenderComponent(*static_cast<RenderComponent*>(&comp));

        inputProperties(comp.getProperties());
    }
}
