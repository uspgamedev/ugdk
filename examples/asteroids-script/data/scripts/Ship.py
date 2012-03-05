from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_input import InputManager, K_w, K_a, K_s, K_d, M_BUTTON_LEFT
from BasicEntity import BasicEntity
from Radio import Radio
from Projectile import Projectile
from math import pi

class Ship (BasicEntity):
    def __init__(self, x, y):
        BasicEntity.__init__(self, x, y, "ship.png", 20.0, 100.0)
        self.radio = Radio()
        self.acceleration = Vector2D(0.0, 0.0)
        self.speed = 50.0                   # |acceleration| in a given frame
        self.max_speed = 100.0              # max |velocity| ship can attain.
        self.projectile_speed = 120         #

    def Update(self, dt):
        self.CheckCommands()
        self.radio.CheckCommands()

        self.velocity = self.velocity + (self.acceleration * dt)
        if (self.velocity.Length() > self.max_speed):
            self.velocity = self.velocity * (self.max_speed/self.velocity.Length())
        self.UpdatePosition(dt)

    def CheckCommands(self):
        input = Engine_reference().input_manager()
        mouse_dir = input.GetMousePosition() - self.node.modifier().offset()
        mouse_dir = mouse_dir.Normalize()
        self.node.modifier().set_rotation( mouse_dir.Angle() - 3*pi/2.0 )
        accel = Vector2D(0.0, 0.0)
        if input.KeyDown(K_w):
            accel += mouse_dir
        if input.KeyDown(K_a):
            left = mouse_dir.Rotate(-pi/2.0)
            left = left.Normalize()
            accel += left
            accel = accel.Normalize()
        if input.KeyDown(K_s):
            accel += -mouse_dir
            accel = accel.Normalize()
        if input.KeyDown(K_d):
            right = mouse_dir.Rotate(pi/2.0)
            right = right.Normalize()
            accel += right
            accel = accel.Normalize()
        accel = accel * self.speed
        self.acceleration = accel

        if input.MousePressed(M_BUTTON_LEFT):
            self.Shoot(mouse_dir)

    def Shoot(self, direction):
        pos = self.node.modifier().offset()
        dir = direction.Normalize() * (self.size.Length()/2.0)
        pos = pos + dir
        vel = self.velocity + (direction.Normalize() * self.projectile_speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, 1.0)
        self.new_objects.append(proj)
        self.radio.PlaySound("fire.wav")

    def HandleCollision(self, target):
      	if target.type == "Planet.Planet":
      		pass
      		
  		#No handler for projectile since that is strictly
  		#"do it only one time", and Projectile will handle it
  		#    same goes for collision with Asteroid
