
require "ugdk.base"
require "ugdk.action"

local engine = ugdk_base.Engine_reference()

local function current_scene()
  return engine:CurrentScene()
end

entity = {}

entity.time = 0

function entity:Update(dt)
  self.time = self.time + dt
  if self.time >= 5 then
    current_scene():Finish()
  end
end

