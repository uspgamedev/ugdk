
require "ugdk.math"

print "============ HEY FROM LUA"

function ls(t)
    for k,v in pairs(t) do
        print(k,v)
    end
end

print = print

v = ugdk_math.Vector2D(1,2)
str = "AMAGAD LUA STRING"
bool = true
integer = 42
number = math.pi

print "============ BYE FROM LUA"

