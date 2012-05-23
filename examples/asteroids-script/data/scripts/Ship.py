from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference, ResourceManager_CreateTextFromLanguageTag
from ugdk.ugdk_input import InputManager, K_w, K_a, K_s, K_d, M_BUTTON_LEFT, K_ESCAPE, M_BUTTON_RIGHT
from BasicEntity import BasicEntity, GetEquivalentValueInRange
from Radio import Radio, SOUND_PATH
from Projectile import Projectile, AntiGravShield
from BarUI import BarUI, BAR_HEIGHT
from Shockwave import Shockwave
from math import pi
from random import randint

from ugdk.ugdk_graphic import Node


class Ship (BasicEntity):
    def __init__(self, x, y):
        BasicEntity.__init__(self, x, y, "images/ship.png", 20.0, 100.0)
        self.radio = Radio()
        self.acceleration = Vector2D(0.0, 0.0)
        self.max_energy = 100.0
        self.energy = self.max_energy
        self.energy_regen_rate = 10.0       # energy per second
        self.shot_cost = 5.0                # energy required to shoot the weakest projectile
        self.max_charge_time = 1.0          # max time that you can charge a shot in seconds
        self.charge_time = 0                # used internally for counting, in seconds
        self.power_range = [0.5, 3.0]       # range in which the shot can be
        self.speed = 400.0                  # |acceleration| in a given frame
        self.max_speed = 200.0              # max |velocity| ship can attain.
        self.projectile_speed = 170         # 
        self.energy_hud = BarUI(self, "energy", Color(0.0,0.0,1.0,1.0), Vector2D(0.0, self.radius+BAR_HEIGHT))
        self.hud_node.AddChild(self.energy_hud.node)

        self.gravshield = AntiGravShield(self, 35)
        self.nameText = Engine_reference().text_manager().GetText("Defiant")
        #self.nameText = ResourceManager_CreateTextFromLanguageTag("ShipName")
        self.nameNode = Node(self.nameText)
        self.node.AddChild(self.nameNode)

    def TakeDamage(self, damage):
        if damage < 0:  return
        self.life -= damage
        if damage > 0:
            sound_name = self.hit_sounds[ randint(0, len(self.hit_sounds)-1) ]
            sound = Engine_reference().audio_manager().LoadSample(SOUND_PATH + sound_name)
            sound.Play()
        if self.life <= 0:
            self.is_destroyed = True
        #print self, "took %s damage, current life = %s" % (damage, self.life)

    def RestoreEnergy(self, amount):
        if amount < 0:  return
        self.energy += amount
        if self.energy > self.max_energy:
            self.energy = self.max_energy

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
        ############
        #if input.KeyDown(K_w):
        #    accel += mouse_dir
        #if input.KeyDown(K_a):
        #    left = mouse_dir.Rotate(-pi/2.0)
        #    left = left.Normalize()
        #    accel += left
        #    accel = accel.Normalize()
        #if input.KeyDown(K_s):
        #    accel += -mouse_dir
        #    accel = accel.Normalize()
        #if input.KeyDown(K_d):
        #    right = mouse_dir.Rotate(pi/2.0)
        #    right = right.Normalize()
        #    accel += right
        #    accel = accel.Normalize()
        #############
        if input.KeyDown(K_w):
            accel += Vector2D(0.0, -1.0)
        if input.KeyDown(K_a):
            accel += Vector2D(-1.0, 0.0)
        if input.KeyDown(K_s):
            accel += Vector2D(0.0, 1.0)
        if input.KeyDown(K_d):
            accel += Vector2D(1.0, 0.0)
        accel = accel.Normalize()
        accel = accel * self.speed
        self.acceleration = accel

        self.gravshield.active = input.MouseDown(M_BUTTON_RIGHT)

        if input.MouseDown(M_BUTTON_LEFT):
            self.charge_time += dt
            if self.charge_time >= self.max_charge_time:
                self.charge_time = self.max_charge_time
        elif not self.gravshield.active:
            if self.energy < self.max_energy:
                self.energy += self.energy_regen_rate * dt

        if input.MouseUp(M_BUTTON_LEFT) and self.charge_time > 0:
            power = GetEquivalentValueInRange(self.charge_time, [0, self.max_charge_time], self.power_range)
            cost = self.shot_cost * (1 + (power * self.charge_time))
            self.Shoot(mouse_dir, power, cost)
            self.charge_time = 0.0
            
        #if input.MousePressed(M_BUTTON_RIGHT):
        #    pos = self.GetPos()
        #    wave = Shockwave(pos.get_x(), pos.get_y(), 4.0, [self.radius, self.radius*5])
        #    wave.AddIDToIgnoreList(self.id)
        #    self.new_objects.append(wave)

    def Shoot(self, direction, power, cost):
        if self.energy < cost:    return
        self.energy -= cost
        pos = self.GetPos()
        dir = direction.Normalize() * 1.15 * (self.radius + Projectile.GetActualRadius(power))
        pos = pos + dir
        vel = self.velocity + (direction.Normalize() * self.projectile_speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, power, True)
        self.new_objects.append(proj)
        self.radio.PlaySound("fire.wav")

    def HandleCollision(self, target):
        if target.type == "Planet":
            pass
        #No handler for projectile since that is strictly
        #"do it only one time", and Projectile will handle it
        #    same goes for collision with Asteroid
