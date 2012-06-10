from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_graphic import Drawable, Node
from ugdk.pyramidworks_collision import CollisionObject, CollisionLogic
from ugdk.pyramidworks_geometry import Circle
from BasicEntity import BasicEntity, EntityInterface, BasicColLogic, getCollisionManager
import Config
import Shockwave
import Animations
import Ship

from random import random, randint, shuffle
from math import pi

#########################################
class PowerUp (BasicEntity):
    def __init__(self, x, y, texture_name, lifetime, effect, name):
        r = 15.0
        BasicEntity.__init__(self, x, y, texture_name, r, 1)
        self.life_hud.node.set_active(False)
        self.max_velocity = 135.0

        self.lifespan = lifetime
        self.lifetime = lifetime
        self.effect = effect
        self.wasApplied = False
        self.blink_time = 0.0

        self.textDrawables = []
        self.textNodes = []
        for node in self.nodes:
            text = Engine_reference().text_manager().GetText(name)
            textNode = Node(text)
            textNode.modifier().set_offset( Vector2D(-text.width()/2.0, 0.0 ) )
            textNode.set_active(False)
            self.textDrawables.append(text)
            self.textNodes.append(textNode)
            node.AddChild(textNode)

    def setupCollisionObject(self):
        self.collision_object = CollisionObject(getCollisionManager(), self)  #initialize collision object, second arg is passed to collisionlogic to handle collisions
        self.collision_object.InitializeCollisionClass("PowerUp")              # define the collision class
        self.geometry = Circle(self.radius)                           #
        self.collision_object.set_shape(self.geometry)                # set our shape
        self.collision_object.AddCollisionLogic("Entity", BasicColLogic(self) )
        self.collision_object.thisown = 0

    def Update(self, dt):
        if not self.wasApplied:
            BasicEntity.Update(self, dt)
        self.lifetime -= dt
        if self.lifetime < self.lifespan*0.15 and not self.wasApplied:
            self.blink_time += dt
            if self.blink_time > self.lifetime/self.lifespan:
                self.blink_time = 0.0
                self.node.set_active( not self.node.active() )
        if self.lifetime < 0:
            self.is_destroyed = True

    def HandleCollision(self, target):
        if target.CheckType("Ship") and not self.wasApplied:
            self.effect.SetTarget(target)
            target.ApplyEffect(self.effect)
            self.wasApplied = True
            self.lifetime = 3.0
            for textNode in self.textNodes:
                textNode.set_active(True)
            self.node.modifier().set_alpha(0.2)
            self.node.set_active(True)
            #TODO: play powerup sound !

###########################
class Effect (EntityInterface):
    def __init__(self, lifetime):
        EntityInterface.__init__(self, 0, 0, 1.0)
        self.is_collidable = False
        self.target = None
        self.lifetime = lifetime
        self.unique_in_target = False #if True, there can be only 1 effect of this type in a entity simultaneously

    def Update(self, dt):
        if not self.is_destroyed and self.target != None and not self.target.is_destroyed:
            self.Apply(dt)
        self.lifetime -= dt
        if self.lifetime < 0 or (self.target != None and self.target.is_destroyed):
            self.is_destroyed = True

    def SetTarget(self, target):
        self.target = target

    def Apply(self, dt):
        pass

#############
class AbsoluteLifeEffect (Effect):
    def __init__(self, lifetime, amount, doHeal=True, isRegen=False):
        Effect.__init__(self, lifetime)
        self.amount = amount
        self.heals = doHeal
        self.regen = isRegen

    def Apply(self, dt):
        value = self.amount
        if self.regen:
            value *= dt
        if not self.heals:
            self.target.TakeDamage(value)
        else:
            self.target.Heal(value)
################
class AbsoluteEnergyEffect (Effect):
    def __init__(self, lifetime, amount, isRegen=False):
        Effect.__init__(self, lifetime)
        self.amount = amount
        self.regen = isRegen

    def Apply(self, dt):
        value = self.amount
        if self.regen:
            value *= dt
        self.target.RestoreEnergy(value)
#################
class MaxValueIncreaseEffect (Effect):
    ENERGY = "energy"
    LIFE = "life"
    def __init__(self, valueTypeName, amount):
        Effect.__init__(self, 0)
        self.amount = amount
        self.valueTypeName = valueTypeName

    def Apply(self, dt):
        if self.valueTypeName == MaxValueIncreaseEffect.ENERGY:
            self.target.set_max_energy( self.target.max_energy + self.amount )
            self.target.energy += self.amount
        elif self.valueTypeName == MaxValueIncreaseEffect.LIFE:
            self.target.set_max_life( self.target.max_life + self.amount )
            self.target.life += self.amount

##################
class PulseDamageIncreaseEffect(Effect):
    def __init__(self, amount):
        Effect.__init__(self, 0)
        self.amount = amount
    def Apply(self, dt):
        self.target.data.pulse_damage += self.amount

#################
class SatelliteEffect(Effect):
    def __init__(self):
        Effect.__init__(self, 10)
        self.sat1 = None
        self.sat2 = None
        self.unique_in_target = True

    def OnSceneAdd(self, scene):
        self.sat1 = Ship.Satellite(self.target, 100, pi/2.0)
        self.sat2 = Ship.Satellite(self.target, 100, 3*pi/2.0)
        self.target.new_objects.append(self.sat1)
        self.target.new_objects.append(self.sat2)

    def Apply(self, dt):
        if (self.sat1.is_destroyed and self.sat2.is_destroyed) or self.target.is_destroyed:
            self.sat1.is_destroyed = True
            self.sat2.is_destroyed = True
            self.lifetime = 0.0
        else:
            self.lifetime = 10.0

    def Update(self, dt):
        Effect.Update(self,dt)
        if self.is_destroyed:
            self.sat1.is_destroyed = True
            self.sat2.is_destroyed = True

#################
class ShieldEffect(Effect):
    def __init__(self, life):
        Effect.__init__(self, 10)
        self.max_life = life
        self.life = life
        self.is_collidable = True
        self.unique_in_target = True
        self.collision_object = CollisionObject(getCollisionManager(), self)
        self.collision_object.InitializeCollisionClass("PowerUp")
        self.geometry = Circle(1.0)
        self.geometry.thisown = 0
        self.collision_object.set_shape( self.geometry )

    def OnSceneAdd(self, scene):
        self.radius = self.target.radius * 1.2
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_name = "images/shockwave.png"
        texture_obj = Engine_reference().resource_manager().texture_container().Load(texture_name, texture_name)
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        self.node.set_drawable(self.shape)
        self.node.modifier().set_alpha(0.5)

        self.geometry = Circle(self.radius)
        self.collision_object.set_shape(self.geometry)
        self.collision_object.AddCollisionLogic("Entity", BasicColLogic(self) )
        

    def Apply(self, dt):
        self.node.modifier().set_offset( self.target.GetPos() )
        if self.life > 0 and not self.target.is_destroyed:
            self.lifetime = 10.0
        else:
            self.lifetime = 0.0

    def HandleCollision(self, coltarget):
        if coltarget.CheckType("Asteroid") or (coltarget.CheckType("Projectile") and coltarget.GetParentID() != self.target.id):
            self.life -= coltarget.GetDamage(self.target.type)
            coltarget.TakeDamage(coltarget.life + 10)
            #print "SHIELD COLLISION %s/%s" % (self.life, self.max_life)

#####################
class ItemAttractorEffect(Effect):
    def __init__(self, lifetime, radius, force):
        Effect.__init__(self, lifetime)
        self.radius = radius
        self.is_collidable = True
        self.unique_in_target = True
        self.force = force
        self.collision_object = CollisionObject(getCollisionManager(), self)
        self.collision_object.InitializeCollisionClass("PowerUp")
        self.geometry = Circle(self.radius)
        self.collision_object.set_shape(self.geometry)
        self.collision_object.AddCollisionLogic("PowerUp", BasicColLogic(self) )
        
    def Apply(self, dt):
        self.node.modifier().set_offset( self.target.GetPos() )

    def HandleCollision(self, coltarget):
        if coltarget.CheckType("PowerUp"): # we can collide with any powerup or collidable effect, however we only affect powerups (the actual item)...
            v = self.GetPos() - coltarget.GetPos()
            v = v.Normalize()
            v = v * self.force
            coltarget.ApplyVelocity(v)

####################
class WeaponPickupEffect(Effect):
    def __init__(self, weapon):
        Effect.__init__(self, 0)
        self.weapon = weapon
    def Apply(self, dt):
        self.target.SetRightWeapon(self.weapon)

####################
class ShockwaveEffect(Effect):
    def __init__(self, shock_lifetime, shock_radius_range, shock_damage, wave_damage):
        Effect.__init__(self, 0)
        self.shock_lifetime = shock_lifetime
        self.shock_radius_range = shock_radius_range
        self.shock_damage = shock_damage    # done once when shockwave hits a target
        self.wave_damage = wave_damage      # done continously while shockwave pushes a target

    def Apply(self, dt):
        pos = self.target.GetPos()
        wave = Shockwave.Shockwave(pos.get_x(), pos.get_y(), self.shock_lifetime, self.shock_radius_range)
        wave.shock_damage = self.shock_damage
        wave.wave_damage = self.wave_damage
        wave.shock_force_factor = 0.4
        wave.AddIDToIgnoreList(self.target.id)
        self.target.new_objects.append(wave)
        exploAnim = Animations.CreateExplosionAtLocation(pos, self.shock_radius_range[1])
        self.target.new_objects.append(exploAnim)

##################
class FractureEffect(Effect):
    def __init__(self):
        Effect.__init__(self, 0)

    def Apply(self, dt):
        scene = Engine_reference().CurrentScene()
        for obj in scene.objects:
            if obj.CheckType("Asteroid"):
                exploAnim = Animations.CreateExplosionAtLocation(obj.GetPos(), obj.radius)
                obj.new_objects.append(exploAnim)
                obj.Break()
