from ugdk.ugdk_graphic import Node, Modifier, Drawable
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
from ugdk.ugdk_action import Entity
from ugdk.pyramidworks_collision import CollisionObject, CollisionLogic
from ugdk.pyramidworks_geometry import Circle
from Radio import SOUND_PATH
from BarUI import BarUI
from random import randint

MANAGER = None

def window_size():
    return Engine_reference().video_manager().video_size()
    
    
def getCollisionManager():
    scene = Engine_reference().CurrentScene()
    #print "Getting COLLISION MANAGER from ", scene
    return scene.collisionManager


class EntityInterface (Entity):
    nextID = 1
    def __init__(self, x, y, radius):
        self.radius = radius  ###
        self.hud_node = Node()
        self.node = Node()   ###
        self.node.modifier().set_offset( Vector2D(x,y) )
        #self.node.thisown = 0
        self.new_objects = []  ###
        self.is_destroyed = False ###
        self.type = str(self.__class__)  ###
        if len(self.type.split("'")) > 1:
            self.type = self.type.split("'")[1]
        self.type = self.type.split(".")[1]
        #print "CLASS TYPE: ", self.type
        self.id = EntityInterface.nextID
        self.is_collidable = True ###
        EntityInterface.nextID += 1

    def CheckType(self, typestr):
        return self.type.count(typestr) > 0

    def ClearNewObjects(self): ###
        self.new_objects = []

    def GetPos(self):
        return self.node.modifier().offset()

    def GetPointsValue(self):
        return 0

    def HandleMapBoundaries(self, pos):
        max = window_size()
        # checking for horizontal map boundaries
        if pos.get_x() < 0.0:
            pos.set_x( max.get_x() + pos.get_x() )
        if pos.get_x() > max.get_x():
            pos.set_x( pos.get_x() - max.get_x() )
        # checking for vertical map boundaries
        if pos.get_y() < 0.0:
            pos.set_y( max.get_y() + pos.get_y() )
        if pos.get_y() > max.get_y():
            pos.set_y( pos.get_y() - max.get_y() )
            
    def Update(self, dt): ###
        pass

    def HandleCollision(self, target): ###
        print self.type, " HandleCollision NOT IMPLEMENTED"
        
    def __repr__(self):
        return "<%s #%s>" % (self.type, self.id)
        
    def __str__(self): return self.__repr__()
    
    
class BasicColLogic(CollisionLogic):
    def __init__(self, entity):
        self.entity = entity
    def Handle(self, data):
        self.entity.HandleCollision(data)

        
class BasicEntity (EntityInterface):
    nextID = 1
    def __init__(self, x, y, texture_name, radius, life):
        EntityInterface.__init__(self, x, y, radius)
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_obj = Engine_reference().resource_manager().texture_container().Load(texture_name, texture_name)
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        #self.shape.thisown = 0
        self.node.set_drawable(self.shape)
        self.velocity = Vector2D(0.0, 0.0)
        self.max_velocity = 5000.0 #length of the maximum velocity - the entity can't achieve a velocity with length greater than this by whatever means
        self.last_velocity = None
        self.last_dt = 0.000001
        self.life = life
        self.max_life = life
        self.hit_sounds = ["hit1.wav", "hit2.wav", "hit3.wav", "hit4.wav"]
        self.life_hud = BarUI(self, "life", Color(1.0,0.0,0.0,1.0), Vector2D(0.0, self.radius))
        self.hud_node.AddChild(self.life_hud.node)
        self.setupCollisionObject()

    def setupCollisionObject(self):
        self.collision_object = CollisionObject(getCollisionManager(), self)  #initialize collision object, second arg is passed to collisionlogic to handle collisions
        self.collision_object.InitializeCollisionClass("Entity")              # define the collision class
        self.geometry = Circle(self.radius)                           #
        self.collision_object.set_shape(self.geometry)                # set our shape
        #finally add collision logics to whatever collision class we want
        self.collision_object.AddCollisionLogic("Entity", BasicColLogic(self) )
        self.collision_object.thisown = 0

    def ApplyEffect(self, effect):
        #since effects are entities too, we just do this
        self.new_objects.append(effect)

    def Update(self, dt): ###
        self.UpdatePosition(dt)
        self.life_hud.Update()
        if self.velocity.Length() > self.max_velocity:
            self.velocity = self.velocity * (self.max_velocity / self.velocity.Length())

    def UpdatePosition(self, dt):
        pos = self.GetPos()
        pos = pos + (self.velocity * dt)
        self.last_velocity = self.velocity
        self.last_dt = dt
        self.HandleMapBoundaries(pos)
        self.node.modifier().set_offset(pos)
        self.hud_node.modifier().set_offset(pos)

    def GetDamage(self, obj_type):
        # returns the amount of damage this object causes on collision with given obj_type
        print self.type, " GetDamage NOT IMPLEMENTED"

    def TakeDamage(self, damage):
        if damage < 0:  return
        self.life -= damage
        if damage > 0:
            sound_name = self.hit_sounds[ randint(0, len(self.hit_sounds)-1) ]
            sound = Engine_reference().audio_manager().LoadSample(SOUND_PATH + sound_name)
            sound.Play()
        if self.life <= 0:
            self.is_destroyed = True
        #print self, "took %s damage, current life = %s [max life =%s]" % (damage, self.life, self.max_life)

    def Heal(self, amount):
        if amount < 0:  return
        self.life += amount
        if self.life > self.max_life:
            self.life = self.max_life
        #print self, "has recovered %s life, current life = %s" % (amount, self.life)
        
    def ApplyVelocity(self, v):
        self.velocity = self.velocity + v

    def ApplyCollisionRollback(self):
        pos = self.GetPos()
        v = self.last_velocity
        if v == None:   v = self.velocity
        pos = pos + (v * -self.last_dt)
        self.HandleMapBoundaries(pos)
        pos = pos + (self.velocity * self.last_dt)
        self.HandleMapBoundaries(pos)
        self.node.modifier().set_offset(pos)
        self.last_velocity = self.velocity
    
    
#################################################
# utility functions
#################################################

# GetEquivalentValueInRange()
###
# given origin_value, which should be a value in the range origin_range ( [origin_range[0], origin_range[1]] )
# and the destination_range, this function returns the equivalent value of origin_value in the destination_range,
# based on the ranges.
###
# if origin_value = X, origin_range = [A, B], destination_range = [C, D], then this function returns V so that:
# (V-C)/(D-C) = (X-A)/(B-A)
#   <=>
# V = C + (D-C)(X-A)/(B-A)
def GetEquivalentValueInRange(origin_value, origin_range, destination_range):
    xa = origin_value - origin_range[0]                 # xa = X - A
    ba = origin_range[1] - origin_range[0]              # ba = B - A
    if ba == 0.0:  #Go POG! Go POG!
        ba = 0.0001
    dc = destination_range[1] - destination_range[0]    # dc = D - C
    r = dc * xa / ba                                    # r = (D-C)(X-A)/(B-A)
    return destination_range[0] + r       # return C + r


#### momentum
# MaVa + MbVb = MaVa' + MbVb'
# 1 = e = (Vb' - Va')/(Va-Vb)
###
# following momentum formulas, returns a pair of the speeds (velocity magnetude) of each entity (in order) after a collision
# NOTE: assumes coefficient = 1.0; which should be enough for now... Severe changes required here if coefficient is another value.
def CalculateAfterSpeedBasedOnMomentum(ent1, ent2):
    m1 = ent1.mass
    m2 = ent2.mass
    v1 = ent1.velocity.Length()
    v2 = ent2.velocity.Length()
    new_v1 = ((2*m2*v2) + (v1 * (m1 - m2))) / (m1 + m2)
    new_v2 = ((2*m1*v1) + (v2 * (m2 - m1))) / (m2 + m1)
    return (new_v1, new_v2)
