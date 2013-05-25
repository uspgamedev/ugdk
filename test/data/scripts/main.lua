require "ugdk.math"

--- Lists the contents of a table to the standard output.
-- 
function ls(t)
  for k,v in pairs(t) do
    print(k,v)
  end
end

function soma(a, b)
    return a + b
end

print = print

v = ugdk_math.Vector2D(1,2)
bool = true
integer = 42
number = math.pi

