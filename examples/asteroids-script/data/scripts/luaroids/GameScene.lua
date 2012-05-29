
require "ugdk.base"
require "ugdk.action"
require "ugdk.input"

local AsteroidsScene = proxy "ugdk::action::Scene"

table.foreach(getmetatable(AsteroidsScene), print)
print "==="
table.foreach(swig, print)

--[[
for k,v in pairs(SWIG.Scene[".fn"]) do
  AsteroidsScene[k] = v
end
]]

function AsteroidsScene:Focus () end
function AsteroidsScene:DeFocus () end
function AsteroidsScene:End () end

function AsteroidsScene:CheckCommands ()
  local input = ugdk_base.Engine_reference():input_manager()
  if input:KeyPressed(ugdk_input.K_ESCAPE) then
    --SWIG.Scene[".fn"].Finish(self)
    self:Finish()
  elseif input:KeyPressed(ugdk_input.K_HOME) then
    --self:Finish()
    --StartupScene()
  end
end

function AsteroidsScene:Update (dt)
  self:CheckCommands()
end

--setmetatable(AsteroidsScene, SWIG.Scene)

function StartupScene ()
  -- does nothing for now.
  ugdk_base.Engine_reference():PushScene(AsteroidsScene)
end
