
require "ugdk.base"
require "ugdk.math"
require "ugdk.spritesheet"

function CreateSpritesheet (path, frame_width, frame_height, hotspot)
  local resources = ugdk_base.Engine_reference():resource_manager()
  local tex = resources:texture_container():Load(path, path)
  local sheet = ugdk_spritesheet.FlexibleSpritesheet(tex)
  sheet:set_frame_size(ugdk_math.Vector2D(frame_width, frame_height))
  sheet:set_hotspot(hotspot)
  resources:spritesheet_container():Insert(path, sheet) 
end

function InitializeSpritesheets ()
  CreateSpritesheet("images/explosion.png", 128, 128, ugdk_math.Vector2D(64,64))
end
