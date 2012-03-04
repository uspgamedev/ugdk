from ugdk.ugdk_graphic import Node, Modifier, Drawable
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
from math import pi

def window_size():
    return Engine_reference().video_manager().video_size()

class Projectile:
    def __init__(self, x, y, velocity, power):
        self.size = Vector2D(10.0, 10.0)
        projectile_texture = Engine_reference().resource_manager().texture_container().Load("projectile.png")
        self.shape = TexturedRectangle( projectile_texture, self.size )
        self.shape.thisown = 0
        self.shape.set_hotspot(Drawable.CENTER)
        self.node = Node( self.shape )
        self.node.thisown = 0
        self.node.modifier().set_offset( Vector2D(x,y) )
        self.new_objects = []
        self.is_destroyed = False
        self.name = "Projectile"
        #### ATTRIBUTES
        self.velocity = velocity
        self.damage = 100.0
        self.power = power
        self.lifetime = 10.0 * power

    def Update(self, dt):
        self.UpdatePosition(dt)
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True

    def UpdatePosition(self, dt):
        pos = self.node.modifier().offset()
        newpos = pos + (self.velocity * dt)

        max = window_size()
        # checking for horizontal map boundaries
        if newpos.get_x() < 0.0:
            newpos.set_x( max.get_x() + newpos.get_x() )
        if newpos.get_x() > max.get_x():
            newpos.set_x( newpos.get_x() - max.get_x() )
        # checking for vertical map boundaries
        if newpos.get_y() < 0.0:
            newpos.set_y( max.get_y() + newpos.get_y() )
        if newpos.get_y() > max.get_y():
            newpos.set_y( newpos.get_y() - max.get_y() )

        self.node.modifier().set_offset(newpos)