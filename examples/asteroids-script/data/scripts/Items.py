from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_graphic import Drawable
from ugdk.pyramidworks_collision import CollisionObject, CollisionLogic
from ugdk.pyramidworks_geometry import Circle
from BasicEntity import EntityInterface, BasicColLogic, getCollisionManager

from random import random, randint, shuffle
from math import pi

#########################################
# Factory Methods
#########################################

def CreateRepairPack(x, y, amount):
    e = AbsoluteLifeEffect(0, amount)
    p = PowerUp(x, y, "images/item.png", 30.0, e)
    return p


#########################################
class PowerUp (EntityInterface):
    def __init__(self, x, y, texture_name, lifetime, effect):
        r = 5.0
        EntityInterface.__init__(self, x, y, r)
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_obj = Engine_reference().resource_manager().texture_container().Load(texture_name, texture_name)
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        self.node.set_drawable(self.shape)

        self.lifetime = lifetime
        self.effect = effect

        self.collision_object = CollisionObject(getCollisionManager(), self)
        self.collision_object.InitializeCollisionClass("PowerUp")
        self.geometry = Circle(self.radius)
        self.collision_object.set_shape(self.geometry)
        self.collision_object.AddCollisionLogic("Entity", BasicColLogic(self) )

    def Update(self, dt):
        EntityInterface.Update(self, dt)
        self.lifetime -= dt
        if self.lifetime < 0:
            self.is_destroyed = True

    def HandleCollision(self, target):
        if target.CheckType("Ship"):
            self.effect.SetTarget(target)
            target.ApplyEffect(self.effect)
            self.is_destroyed = True
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
    def __init__(self, lifetime, amount, doHeal=True):
        Effect.__init__(self, lifetime)
        self.amount = amount
        self.heals = doHeal

    def Apply(self, dt):
        if not self.heals:
            self.target.TakeDamage(self.amount)
        else:
            self.target.Heal(self.amount)