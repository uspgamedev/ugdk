
require "ugdk.base"
require "ugdk.action"

local AsteroidsScene = {}

function AsteroidsScene:Focus () end
function AsteroidsScene:DeFocus () end
function AsteroidsScene:End () end

function AsteroidsScene:Update (dt)
  
end

setmetatable(AsteroidsScene, SWIG.Scene)

function StartupScene ()
  -- does nothing for now.
  ugdk_base.Engine_reference():PushScene(AsteroidsScene)
end
