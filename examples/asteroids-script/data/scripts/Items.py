from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_graphic import Drawable, Node
from ugdk.pyramidworks_collision import CollisionObject, CollisionLogic
from ugdk.pyramidworks_geometry import Circle
from BasicEntity import EntityInterface, BasicColLogic, getCollisionManager
import Config

from random import random, randint, shuffle
from math import pi

#########################################
class PowerUp (EntityInterface):
    def __init__(self, x, y, texture_name, lifetime, effect, name):
        r = 15.0
        EntityInterface.__init__(self, x, y, r)
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_obj = Engine_reference().resource_manager().texture_container().Load(texture_name, texture_name)
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        self.node.set_drawable(self.shape)

        self.lifetime = lifetime
        self.effect = effect
        self.wasApplied = False

        self.collision_object = CollisionObject(getCollisionManager(), self)
        self.collision_object.InitializeCollisionClass("PowerUp")
        self.geometry = Circle(self.radius)
        self.collision_object.set_shape(self.geometry)
        self.collision_object.AddCollisionLogic("Entity", BasicColLogic(self) )

        self.textNode = Node()
        self.textDrawable = Engine_reference().text_manager().GetText(name)
        self.textNode.set_drawable( self.textDrawable )
        self.textNode.modifier().set_offset( Vector2D(-self.textDrawable.width()/2.0, 0.0 ) )
        self.textNode.set_active(False)
        self.node.AddChild(self.textNode)
        

    def Update(self, dt):
        EntityInterface.Update(self, dt)
        self.lifetime -= dt
        if self.lifetime < 0:
            self.is_destroyed = True

    def HandleCollision(self, target):
        if target.CheckType("Ship") and not self.wasApplied:
            self.effect.SetTarget(target)
            target.ApplyEffect(self.effect)
            self.wasApplied = True
            self.lifetime = 3.0
            self.textNode.set_active(True)
            self.node.modifier().set_alpha(0.2)
            #TODO: play powerup sound !

###########################
class Effect (EntityInterface):
    def __init__(self, lifetime):
        EntityInterface.__init__(self, 0, 0, 1.0)
        self.is_collidable = False
        self.target = None
        self.lifetime = lifetime

    def Update(self, dt):
        if not self.is_destroyed and self.target != None and not self.target.is_destroyed:
            self.Apply(dt)
        self.lifetime -= dt
        if self.lifetime < 0:
            self.is_destroyed = True

    def SetTarget(self, target):
        self.target = target

    def Apply(self, dt):
        pass


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
