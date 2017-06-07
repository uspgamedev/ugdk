#include <ugdk/action/json2spriteanimation.h>

#include <ugdk/system/exceptions.h>
#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>
#include <ugdk/system/compatibility.h>
#include <libjson.h>

namespace ugdk {
namespace action {

namespace {
    std::unique_ptr<SpriteAnimationFrame> build_frame(const JSONNode& jsondata) {
        auto frame = MakeUnique<SpriteAnimationFrame>(jsondata["name"].as_string().c_str());

		if (jsondata.find("color") != jsondata.end()) {
			auto&& colorjson = jsondata["color"];
			frame->effect().set_color(structure::Color(
				colorjson.at(0).as_float(),
				colorjson.at(1).as_float(),
				colorjson.at(2).as_float(),
				colorjson.at(3).as_float()
			));
		}

		auto visible = jsondata.find("visible");
		if (visible != jsondata.end()) {
			frame->effect().set_visible(visible->as_bool());
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
        frame->geometry() = math::Geometry(offset, scale, rot);

        ugdk::Mirror mirror = ugdk::MIRROR_NONE;

        auto mirrorh = jsondata.find("mirrorh");
        if (mirrorh != jsondata.end() && mirrorh->as_bool())
            mirror |= ugdk::MIRROR_HFLIP;

        auto mirrorv = jsondata.find("mirrorv");
        if (mirrorv != jsondata.end() && mirrorh->as_bool())
                mirror |= ugdk::MIRROR_VFLIP;
        frame->set_mirror(mirror);

        auto period = jsondata.find("period");
        if (period != jsondata.end())
            frame->set_period(period->as_float());

        return frame;
    }

    std::unordered_map<json_string, SpriteAnimation::AnimationRepeat> repeat_string_to_enum = {
        { "loop", SpriteAnimation::AnimationRepeat::LOOP },
        { "none", SpriteAnimation::AnimationRepeat::NONE },
    };
}

std::shared_ptr<SpriteAnimationTable> LoadSpriteAnimationTableFromFile(const std::string& filepath) {
    auto file = filesystem::manager()->OpenFile(filepath);
    if (!file)
        return nullptr;

    auto contents = json_string(file->GetContents().c_str());
    if (!libjson::is_valid(contents))
        throw system::BaseException("Invalid json: %s\n", filepath.c_str());

    auto json_node = libjson::parse(contents);
    SpriteAnimationTable* table = new SpriteAnimationTable(json_node.size());
    for (const auto& animation_json : json_node) {
        auto element = MakeUnique<SpriteAnimation>();

        auto frames_array_json = animation_json;
        if (animation_json.type() == JSON_NODE) {
            frames_array_json = animation_json["frames"];

			auto repeat = animation_json.find("repeat");
            if (repeat != animation_json.end()) {
                element->set_repeat(repeat_string_to_enum.at(repeat->as_string()));
            }
        }

        element->frames().reserve(frames_array_json.size());
        for (const auto& frame_json : frames_array_json) {
            element->frames().emplace_back(build_frame(frame_json));
        }

        table->Add(animation_json.name().c_str(), std::move(element));
    }

    return table;
}

}  // namespace action
}  // namespace ugdk

