#include <ugdk/action/spritetypes.h>

#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/system/engine.h>
#include <JSONWorker.h>

namespace ugdk {
namespace action {
    
namespace {
    action::SpriteAnimationFrame* build_frame(const JSONNode& jsondata) {
        action::SpriteAnimationFrame* frame = new action::SpriteAnimationFrame(jsondata["name"].as_string());

        if (jsondata.find("color") != jsondata.end()) {
            auto&& colorjson = jsondata["color"];
            frame->effect().set_color(Color(
                colorjson.at(0).as_float(),
                colorjson.at(1).as_float(),
                colorjson.at(2).as_float(),
                colorjson.at(3).as_float()
            ));
        }

        // Geometry
        math::Vector2D offset;
        math::Vector2D scale(1.0, 1.0);
        double rot = 0.0;
        if (jsondata.find("position") != jsondata.end()) {
            auto&& json = jsondata["position"];
            offset.x = json.at(0).as_float();
            offset.y = json.at(1).as_float();
        }
        if (jsondata.find("scale") != jsondata.end()) {
            auto&& json = jsondata["scale"];
            scale.x = json.at(0).as_float();
            scale.y = json.at(1).as_float();
        }
        if (jsondata.find("rotation") != jsondata.end()) {
            rot = jsondata.at("rotation").as_float();
        }
        frame->geometry() = graphic::Geometry(offset, scale, rot);

        ugdk::Mirror mirror = ugdk::MIRROR_NONE;
        try {
            if (jsondata.at("mirrorh").as_bool())
                mirror |= ugdk::MIRROR_HFLIP;
        } catch (std::out_of_range) {}
        try {
            if (jsondata.at("mirrorv").as_bool())
                mirror |= ugdk::MIRROR_VFLIP;
        } catch (std::out_of_range) {}
        frame->set_mirror(mirror);

        return frame;
    }
}

action::SpriteAnimationTable* LoadSpriteAnimationTableFromFile(const std::string& filepath) {
    auto json_node = JSONWorker::parse(system::GetFileContents(filepath));

    action::SpriteAnimationTable* table = new action::SpriteAnimationTable(json_node.size());
    for (const auto& animation_json : json_node) {

        action::SpriteAnimation* element = new action::SpriteAnimation;
        element->set_period(animation_json["period"].as_float());
        for (const auto& frame_json : animation_json["frames"]) {
            element->Add(build_frame(frame_json));
        }

        table->Add(animation_json.name(), element);
    }

    return table;
}

}  // namespace action
}  // namespace ugdk

