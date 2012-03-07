from ugdk.ugdk_math import Vector2D
from Gravity import GravityWell
from math import pi


class Shockwave (GravityWell):
    def __init__(self, x, y, lifetime, radius_range)
        self.lifetime = lifetime
        GravityWell.__init__(self, x, y, r)

    def Update(self, dt):
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True
