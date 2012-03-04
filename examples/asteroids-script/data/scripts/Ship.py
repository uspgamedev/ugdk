from ugdk.ugdk_graphic import Node, Modifier, Drawable
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
from ugdk.ugdk_input import InputManager, K_w, K_a, K_s, K_d, M_BUTTON_LEFT
from math import pi

def window_size():
    return Engine_reference().video_manager().video_size()

class Ship:
    def __init__(self, x, y):
        self.size = Vector2D(50.0, 50.0)
        ship_texture = Engine_reference().resource_manager().texture_container().Load("ship.png")
        self.shape = TexturedRectangle( ship_texture, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        self.node = Node( self.shape )
        self.node.modifier().set_offset( Vector2D(x,y) )
        self.velocity = Vector2D(0.0, 0.0)
        self.acceleration = Vector2D(0.0, 0.0)
        self.speed = 1.0
        self.max_speed = 50.0
    def Update(self, dt):
        self.CheckCommands()
        self.UpdatePosition(dt)

    def UpdatePosition(self, dt):
        pos = self.node.modifier().offset()
        self.velocity = self.velocity + (self.acceleration * dt)
        if (self.velocity.Length() > self.max_speed):
            self.velocity = self.velocity * (self.max_speed/self.velocity.Length())
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

    def CheckCommands(self):
        input = Engine_reference().input_manager()
        mouse_dir = input.GetMousePosition() - self.node.modifier().offset()
        mouse_dir.Normalize()
        self.node.modifier().set_rotation( mouse_dir.Angle() - 3*pi/2.0 )
        accel = Vector2D(0.0, 0.0)
        if input.KeyDown(K_w):
            accel += mouse_dir
        if input.KeyDown(K_a):
            left = mouse_dir.Rotate(-pi/2.0)
            left.Normalize()
            accel += left
            accel.Normalize()
        if input.KeyDown(K_s):
            accel += -mouse_dir
            accel.Normalize()
        if input.KeyDown(K_d):
            right = mouse_dir.Rotate(pi/2.0)
            right.Normalize()
            accel += right
            accel.Normalize()
        accel = accel * self.speed
        self.acceleration = accel