from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_graphic import Drawable
from Gravity import GravityWell, GetGravForce
from BasicEntity import GetEquivalentValueInRange
from math import pi


class Shockwave (GravityWell):
    def __init__(self, x, y, lifetime, radius_range):
        self.max_lifetime = lifetime
        self.lifetime = 0.0
        self.radius_range = radius_range
        GravityWell.__init__(self, x, y, radius_range[0])
        self.is_antigrav = True
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_obj = Engine_reference().resource_manager().texture_container().Load("shockwave.png")
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        self.shape.thisown = 0
        self.node.set_drawable(self.shape)
        self.scale_range = [1, radius_range[1]/radius_range[0]]

    def SetRadius(self, r):
        GravityWell.SetRadius(self, r)
        #self.size.set_x(self.radius * 2)
        #self.size.set_y(self.radius * 2)
        #self.shape.set_size(self.size)
        scale = GetEquivalentValueInRange(r, self.radius_range, self.scale_range)
        self.node.modifier().set_scale( Vector2D(scale, scale) )
        
    def Update(self, dt):
        self.lifetime += dt
        r = GetEquivalentValueInRange(self.lifetime, [0, self.max_lifetime], self.radius_range)
        self.SetRadius(r)
        if self.lifetime > self.max_lifetime:
            #gotta destroy this thing
            self.is_destroyed = True
            
    def HandleCollision(self, target):
        ignore_types = ["Gravity.GravityWell", "Planet.Planet", "Shockwave.Shockwave"]
        if target.type in ignore_types or target.id in self.ignore_ids:
            return 
        #print self, "is affecting", target
        grav_vec = self.GetPos() - target.GetPos()
        dist = grav_vec.Length()

        #GravForce = GetGravForce(self.mass, dist)
        current_r_range = [self.radius, self.radius_range[0]]
        current_gravforce_range = [GetGravForce(self.mass, r) for r in current_r_range]
        GravForce = GetEquivalentValueInRange(dist, current_r_range, current_gravforce_range )
        
        grav_vec = grav_vec.Normalize()
        if self.is_antigrav:
            grav_vec = grav_vec * -1

        grav_vec = grav_vec * GravForce

        target.ApplyVelocity(grav_vec)
