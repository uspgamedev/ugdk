from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference, ResourceManager_CreateTextFromLanguageTag
from ugdk.ugdk_input import InputManager, K_w, K_a, K_s, K_d, M_BUTTON_LEFT, K_ESCAPE, M_BUTTON_RIGHT
from BasicEntity import BasicEntity, GetEquivalentValueInRange
from Radio import Radio, SOUND_PATH
import Weapons
from BarUI import BarUI, BAR_HEIGHT
from Shockwave import Shockwave
from math import pi
from random import randint

from ugdk.ugdk_graphic import Node

class ShipData:
    def __init__(self, max_life, max_energy, pulse_damage):
        self.max_life = max_life
        self.max_energy = max_energy
        self.pulse_damage = pulse_damage
    def __repr__(self):
        return "{ShipData: [MaxLife=%s][MaxEnergy=%s][PulseDmg=%s]}" % (self.max_life, self.max_energy, self.pulse_damage)
    def __str__(self): return self.__repr__()
        

class Ship (BasicEntity):
    def __init__(self, x, y, data):
        BasicEntity.__init__(self, x, y, "images/ship.png", 20.0, data.max_life)
        self.radio = Radio()
        self.acceleration = Vector2D(0.0, 0.0)
        self.data = data
        self.max_energy = self.data.max_energy
        self.energy = self.max_energy
        self.energy_regen_rate = 10.0       # energy per second
        self.speed = 400.0                  # |acceleration| in a given frame
        self.max_speed = 200.0              # max |velocity| ship can attain.
        self.energy_hud = BarUI(self, "energy", Color(0.0,0.0,1.0,1.0), Vector2D(0.0, self.radius+BAR_HEIGHT))
        self.hud_node.AddChild(self.energy_hud.node)

        self.pulse_weapon = Weapons.Pulse()
        self.right_weapon = Weapons.ShockBomb() #AntiGravShield(35)
        self.pulse_weapon.Activate(self)
        self.right_weapon.Activate(self)

    def set_max_life(self, value):
        self.data.max_life = value
        self.max_life = value

    def set_max_energy(self, value):
        self.data.max_energy = value
        self.max_energy = value

    def RestoreEnergy(self, amount):
        if amount < 0:  return
        self.energy += amount
        if self.energy > self.max_energy:
            self.energy = self.max_energy

    def TakeEnergy(self, amount):
        if amount < 0:  return
        self.energy -= amount
        if self.energy < 0:
            self.energy = 0.0

    def SetRightWeapon(self, weapon):
        if self.right_weapon != None:
            self.right_weapon.Dismantle()
        self.right_weapon = weapon
        self.right_weapon.Activate(self)

    def GetDirection(self):
        return Engine_reference().input_manager().GetMousePosition()

    def Update(self, dt):
        self.CheckCommands(dt)
        self.radio.CheckCommands()

        self.velocity = self.velocity + (self.acceleration * dt)
        if (self.velocity.Length() > self.max_speed):
            self.velocity = self.velocity * (self.max_speed/self.velocity.Length())
        self.UpdatePosition(dt)
        self.life_hud.Update()
        self.energy_hud.Update()
        self.CleanUpActiveEffects()

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

        weaponFiring = self.pulse_weapon.Toggle(input.MouseDown(M_BUTTON_LEFT), dt)
        if self.right_weapon != None:
            weaponFiring = weaponFiring or self.right_weapon.Toggle(input.MouseDown(M_BUTTON_RIGHT), dt)

        if not weaponFiring:
            if self.energy < self.max_energy:
                self.energy += self.energy_regen_rate * dt

    def HandleCollision(self, target):
        pass
        #other entities handle collision with Ship

###################

class Satellite(BasicEntity):
    def __init__(self, parent, life, starting_angle):
        self.parent = parent
        x = parent.GetPos().get_x()
        y = parent.GetPos().get_y()
        BasicEntity.__init__(self, x, y, "images/satellite.png", parent.radius/2.0, life)
        self.orbit_angle = starting_angle
        self.angle_speed = pi/2.5  # angle speed in radians per second
        self.turret = Weapons.Turret(self, "Asteroid", 0.6, 150.0, 0.6, Color(0.0, 1.0, 0.1, 0.7))
        self.turret.hitsFriendlyToParent = False
        self.turret.hitsSameClassAsParent = False

    def CalculateOrbitPos(self):
        pos = self.parent.GetPos()
        direction = Vector2D(0, 1).Rotate(self.orbit_angle).Normalize()
        orbit = direction * (self.parent.radius + self.radius + 7.0)
        return orbit + pos

    def Update(self, dt):
        if self.parent.is_destroyed:
            self.is_destroyed = True
            return
        self.node.modifier().set_offset(self.CalculateOrbitPos() )
        self.orbit_angle += self. angle_speed * dt
        if self.orbit_angle > 2*pi:
            self.orbit_angle -= 2*pi
        self.velocity = self.velocity * 0.0
        self.turret.Update(dt)
        BasicEntity.Update(self, dt)
        
    def HandleCollision(self, target):
        pass
