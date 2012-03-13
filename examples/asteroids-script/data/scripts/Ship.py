from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
from ugdk.ugdk_input import InputManager, K_w, K_a, K_s, K_d, M_BUTTON_LEFT, K_ESCAPE, M_BUTTON_RIGHT
from BasicEntity import BasicEntity, GetEquivalentValueInRange
from Radio import Radio
from Projectile import Projectile
from BarUI import BarUI, BAR_HEIGHT
from Shockwave import Shockwave
from math import pi


class Ship (BasicEntity):
    def __init__(self, x, y):
        BasicEntity.__init__(self, x, y, "ship.png", 20.0, 100.0)
        self.radio = Radio()
        self.acceleration = Vector2D(0.0, 0.0)
        self.max_energy = 100.0
        self.energy = self.max_energy
        self.energy_regen_rate = 10.0       # energy per second
        self.shot_cost = 5.0                # energy required to shoot the weakest projectile
        self.max_charge_time = 5.0          # max time that you can charge a shot in seconds
        self.charge_time = 0                # used internally for counting, in seconds
        self.power_range = [0.5, 3.0]       # range in which the shot can be
        self.speed = 100.0                  # |acceleration| in a given frame
        self.max_speed = 150.0              # max |velocity| ship can attain.
        self.projectile_speed = 170         # 
        self.energy_hud = BarUI(self, "energy", Color(0.0,0.0,1.0,1.0), Vector2D(0.0, self.radius+BAR_HEIGHT))

    def Update(self, dt):
        self.CheckCommands(dt)
        self.radio.CheckCommands()

        self.velocity = self.velocity + (self.acceleration * dt)
        if (self.velocity.Length() > self.max_speed):
            self.velocity = self.velocity * (self.max_speed/self.velocity.Length())
        self.UpdatePosition(dt)
        self.life_hud.Update()
        self.energy_hud.Update()

    def CheckCommands(self, dt):
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

        
        if input.MouseDown(M_BUTTON_LEFT):
            self.charge_time += dt
            if self.charge_time >= self.max_charge_time:
                self.charge_time = self.max_charge_time
        else:
            if self.energy < self.max_energy:
                self.energy += self.energy_regen_rate * dt

        if input.MouseUp(M_BUTTON_LEFT) and self.charge_time > 0:
            power = GetEquivalentValueInRange(self.charge_time, [0, self.max_charge_time], self.power_range)
            cost = self.shot_cost * (1 + (power * self.charge_time))
            self.Shoot(mouse_dir, power, cost)
            self.charge_time = 0.0
            
        if input.MousePressed(M_BUTTON_RIGHT):
            pos = self.GetPos()
            wave = Shockwave(pos.get_x(), pos.get_y(), 4.0, [self.radius, self.radius*5])
            wave.AddIDToIgnoreList(self.id)
            self.new_objects.append(wave)

    def Shoot(self, direction, power, cost):
        if self.energy < cost:    return
        self.energy -= cost
        pos = self.GetPos()
        dir = direction.Normalize() * 1.15 * (self.radius + Projectile.GetActualRadius(power))
        pos = pos + dir
        vel = self.velocity + (direction.Normalize() * self.projectile_speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, power)
        self.new_objects.append(proj)
        self.radio.PlaySound("fire.wav")

    def HandleCollision(self, target):
        if target.type == "Planet.Planet":
            pass
        #No handler for projectile since that is strictly
        #"do it only one time", and Projectile will handle it
        #    same goes for collision with Asteroid
