from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference, Color
from BasicEntity import BasicEntity, RangeCheck, GetEquivalentValueInRange
from Animations import CreateExplosionFromCollision
import Shockwave

from random import random
from math import pi

class Projectile (BasicEntity):
    base_radius = 5.0
    @staticmethod
    def GetActualRadius(power):
        return Projectile.base_radius * power

    def __init__(self, x, y, velocity, power, damage = 25.0, isFromPlayer=False):
        self.parent = None #game entity that "owns" this projectile
        self.power = power
        self.damage = damage * power
        self.original_damage = self.damage
        self.lifetime = 10.0 * power
        self.original_radius = Projectile.GetActualRadius(power)
        BasicEntity.__init__(self, x, y, "images/projectile.png", self.original_radius, self.damage)
        for shape in self.shapes:
            shape.set_hotspot( Vector2D(32.0, 32.0) )
            shape.set_size( Vector2D(64, 128) )  # original projectile.png size
        self.on_hit_events = []
        
        self.isFromPlayer = isFromPlayer
        self.hitsFriendlyToParent = True
        self.hitsSameClassAsParent = True
        # scale:
        # base_radius <=> 0.5 (scale value)
        # 
        # radius <=> scale
        scale = self.radius * 0.20 / Projectile.base_radius
        for node in self.nodes:
            node.modifier().set_scale( Vector2D(scale, scale) )
            node.modifier().set_rotation( velocity.Angle() - 3*pi/2.0 )
        self.velocity = velocity
        self.value = 0
        self.life_hud.node.set_active(False)

    def Update(self, dt):
        self.UpdatePosition(dt)
        for node in self.nodes:
            node.modifier().set_rotation( self.velocity.Angle() - 3*pi/2.0 )
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True

    def SetParent(self, parent):
        self.parent = parent

    def GetParentID(self):
        if self.parent != None:
            return self.parent.id
        return -1

    def TakeDamage(self, damage):
        BasicEntity.TakeDamage(self, damage)
        if not self.is_destroyed:
            self.radius = self.original_radius * self.life / self.original_damage
            scale = self.radius * 0.20 / Projectile.base_radius
            self.damage = self.life
            self.node.modifier().set_scale( Vector2D(scale, scale) )

    def GetDamage(self, obj_type):
        if obj_type == "Planet":
            return self.damage * 0.05
        return self.damage

    def GetPointsValue(self):
        return self.value

    def AddOnHitEvent(self, function):
        self.on_hit_events.append(function)

    def CallOnHitEvents(self, target):
        for f in self.on_hit_events:
            f(self, target)

    def IsParentFriendlyToEntity(self, ent):
        if self.parent == None: return False

        p = self.parent
        while True:
            if ent.id == p.id:
                return True
            if hasattr(p, "parent"):
                p = p.parent
            else:
                break
        return False

    def HandleCollision(self, target):
        if self.isFromPlayer and target.CheckType("Asteroid"):
            self.value = self.life / 2

        if not self.hitsFriendlyToParent and self.IsParentFriendlyToEntity(target):
            return
        if not self.hitsSameClassAsParent and target.type == self.parent.type:
            return

        if target.CheckType("Projectile"):
            # collision between projectiles, destroy both
            target.TakeDamage(self.GetDamage(target.type))
            if target.is_destroyed:
                target.CallOnHitEvents(self)
            #self.is_destroyed = True
            CreateExplosionFromCollision(self, target, self.radius*5)
            #print "Projectiles exploding..."
        elif target.CheckType("Ship") or target.CheckType("Asteroid"):
            target.TakeDamage(self.GetDamage(target.type))
            target.ApplyVelocity(self.velocity * (0.1*self.power))
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, target.radius)
            self.CallOnHitEvents(target)
            #print "Projectile damaging ", target.type
        elif target.CheckType("Planet"):
            target.TakeDamage(self.GetDamage(target.type))
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, target.radius*0.7)
            self.CallOnHitEvents(target)
            #print "Projectile impacted planet"
        elif target.CheckType("Satellite") and not self.isFromPlayer:
            target.TakeDamage(self.GetDamage(target.type))
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, target.radius)
            self.CallOnHitEvents(target)
            
#########################
class Turret:
    def __init__(self, parent, target_type, cooldown, speed, power, color=None):
        self.parent = parent
        self.cooldown = cooldown
        self.elapsed = 0.0
        self.speed = speed
        self.power = power
        self.color = color
        self.firing_angle_offset = 2.5
        self.hitsFriendlyToParent = True
        self.hitsSameClassAsParent = True
        self.rangeCheck = RangeCheck(0, 0, 500.0, target_type)
        self.rangeCheck.AttachToEntity(parent)
    
    def Update(self, dt):
        self.elapsed += dt
        if self.elapsed > self.cooldown:
            self.elapsed = 0.0
            target = self.GetTarget()
            if target != None:
                self.Shoot( target )

    def GetTarget(self):
        return self.rangeCheck.GetTarget()

    def GetShootingAngle(self):
        r = random()
        r = r * (self.firing_angle_offset*2)
        return r - self.firing_angle_offset

    def Shoot(self, target):
        pos = self.parent.GetPos()
        dir = target.GetPos() - pos
        dir = dir.Normalize()
        dir = dir * 1.1 * (self.parent.radius + Projectile.GetActualRadius(self.power))
        pos = pos + dir
        vel = target.velocity + (dir.Normalize() * (self.speed + self.parent.velocity.Length()))
        angle = self.GetShootingAngle() #angle is in degrees
        angle = angle * pi / 180.0
        vel = vel.Rotate( angle ) #we need angle in radians
        proj = Projectile(pos.get_x(), pos.get_y(), vel, self.power)
        proj.SetParent(self.parent)
        proj.hitsFriendlyToParent = self.hitsFriendlyToParent
        proj.hitsSameClassAsParent = self.hitsSameClassAsParent
        if self.color != None:
            proj.node.modifier().set_color( self.color )
        self.parent.new_objects.append(proj)
        if hasattr(target, "radio"): #yay pog
            target.radio.PlaySound("fire.wav")


###################################
class Weapon:
    def __init__(self):
        self.parent = None
    def Activate(self, parent):
        self.parent = parent
    def Toggle(self, active, dt):
        return False # return -> boolean indicating if weapon succesfully fired
    def Dismantle(self):
        pass # this function should perform any action necessary to kill/destroy/remove this weapon.

########
class Pulse (Weapon):
    def __init__(self):
        self.shot_cost = 5.0                # energy required to shoot the weakest projectile
        self.max_charge_time = 1.0          # max time that you can charge a shot in seconds
        self.charge_time = 0                # used internally for counting, in seconds
        self.power_range = [0.5, 3.0]       # range in which the shot can be
        self.projectile_speed = 170         #

    def Toggle(self, active, dt):
        if active:
            self.charge_time += dt
            if self.charge_time >= self.max_charge_time:
                self.charge_time = self.max_charge_time
        if not active and self.charge_time > 0:
            power = GetEquivalentValueInRange(self.charge_time, [0, self.max_charge_time], self.power_range)
            cost = self.shot_cost * (1 + (power * self.charge_time))
            mouse_dir = Engine_reference().input_manager().GetMousePosition() - (Engine_reference().video_manager().video_size() * 0.5) #self.parent.GetPos()
            mouse_dir = mouse_dir.Normalize()
            self.charge_time = 0.0
            return self.Shoot(mouse_dir, power, cost)
        return False
        
    def Shoot(self, direction, power, cost):
        if self.parent.energy < cost:    return False
        self.parent.TakeEnergy(cost)
        pos = self.parent.GetPos()
        dir = direction.Normalize() * 1.15 * (self.parent.radius + Projectile.GetActualRadius(power))
        pos = pos + dir
        vel = self.parent.velocity + (direction.Normalize() * self.projectile_speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, power, self.parent.data.pulse_damage, True)
        proj.SetParent(self.parent)
        proj.node.modifier().set_color( Color(0.0, 0.5, 1.0, 0.9) )
        self.parent.new_objects.append(proj)
        self.parent.radio.PlaySound("fire.wav")
        return True

#########
from Gravity import GravityWell

class AntiGravShield(GravityWell, Weapon):
    def __init__(self, energyCostPerSec):
        self.energyCostPerSec = energyCostPerSec
        GravityWell.__init__(self, 0, 0, 1)
        self.is_antigrav = True
    
    def Activate(self, parent):
        Weapon.Activate(self, parent)
        self.SetBaseRadius(parent.radius*3)
        self.mass *= 10
        self.AddIDToIgnoreList(parent.id)
        parent.new_objects.append(self)

    def Toggle(self, active, dt):
        self.active = active
        self.Update(dt)
        return self.active

    def Update(self, dt):
        if self.parent.is_destroyed:
            self.is_destroyed = True
        else:
            GravityWell.Update(self, dt)
            self.node.modifier().set_offset( self.parent.GetPos() )
            if self.active and hasattr(self.parent, "energy"):
                if self.parent.energy < self.parent.max_energy*0.05:
                    self.active = False
                else:
                    self.parent.TakeEnergy( self.energyCostPerSec * dt )

    def Dismantle(self):
        self.is_destroyed = True
        self.active = False

##########
class ShockBomb(Weapon):
    def __init__(self):
        self.energy_cost = 50.0
        self.projectile_speed = 170.0
        self.shock_lifetime = 1.0
        self.shock_radius_range = [5.0, 180.0]
        self.can_shoot = True
        self.shock_damage = 80.0    # done once when shockwave hits a target
        self.wave_damage = 0.5      # done continously while shockwave pushes a target

    def Toggle(self, active, dt):
        if active and self.can_shoot:
            self.can_shoot = False
            mouse_dir = Engine_reference().input_manager().GetMousePosition() - self.parent.GetPos()
            mouse_dir = mouse_dir.Normalize()
            return self.Shoot(mouse_dir)
        elif not active:
            self.can_shoot = True
        return False
 
    def Shoot(self, direction):
        if self.parent.energy < self.energy_cost:    return False
        self.parent.TakeEnergy(self.energy_cost)
        power = 1.0
        pos = self.parent.GetPos()
        dir = direction.Normalize() * 1.15 * (self.parent.radius + Projectile.GetActualRadius(power))
        pos = pos + dir
        vel = self.parent.velocity + (direction.Normalize() * self.projectile_speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, power, 10.0, True)
        proj.SetParent(self.parent)
        proj.AddOnHitEvent(self.WarheadDetonation)
        proj.node.modifier().set_color( Color(1.0, 1.0, 0.1, 1.0) )
        self.parent.new_objects.append(proj)
        self.parent.radio.PlaySound("fire.wav")
        return True

    def WarheadDetonation(self, projectile, target):
        pos = projectile.GetPos()
        wave = Shockwave.Shockwave(pos.get_x(), pos.get_y(), self.shock_lifetime, self.shock_radius_range)
        wave.shock_damage = self.shock_damage
        wave.wave_damage = self.wave_damage
        wave.shock_force_factor = 0.05
        wave.AddIDToIgnoreList(self.parent.id)
        self.parent.new_objects.append(wave)

#########
class Blackhole(Weapon):
    def __init__(self):
        pass

#########
class Hyperspace(Weapon):
    def __init__(self):
        self.energy_cost = 40.0
        self.cooldown = 5.0
        self.time_elapsed = 0.0
        self.enabled = True

    def Toggle(self, active, dt):
        if self.enabled:
            if active:
                mouse_pos = Engine_reference().input_manager().GetMousePosition()
                return self.Engage(mouse_pos)
        else:
            self.time_elapsed += dt
            if self.time_elapsed > self.cooldown:
                self.enabled = True
                self.time_elapsed = 0.0
        return False

    def Engage(self, pos):
        if self.parent.energy < self.energy_cost:    return False
        self.parent.TakeEnergy(self.energy_cost)
        self.parent.node.modifier().set_offset(pos)
        self.enabled = False
        return True
