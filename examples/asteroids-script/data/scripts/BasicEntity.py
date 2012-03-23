from ugdk.ugdk_graphic import Node, Modifier, Drawable
from ugdk.ugdk_drawable import TexturedRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
from Radio import SOUND_PATH
from BarUI import BarUI
from random import randint


def window_size():
    return Engine_reference().video_manager().video_size()

#   Functions or attributes marked with ### means they're part of the 
#   C++ ScriptEntity interface

class EntityInterface:
    nextID = 1
    def __init__(self, x, y, radius):
        self.radius = radius  ###
        self.node = Node()   ###
        self.node.modifier().set_offset( Vector2D(x,y) )
        self.node.thisown = 0
        self.new_objects = []  ###
        self.is_destroyed = False ###
        self.type = str(self.__class__)  ###
        self.id = EntityInterface.nextID
        EntityInterface.nextID += 1

    def ClearNewObjects(self): ###
        self.new_objects = []

    def GetPos(self):
        return self.node.modifier().offset()

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
    
    
class BasicEntity (EntityInterface):
    nextID = 1
    def __init__(self, x, y, texture_name, radius, life):
        EntityInterface.__init__(self, x, y, radius)
        self.size = Vector2D(self.radius*2, self.radius*2)
        texture_obj = Engine_reference().resource_manager().texture_container().Load(texture_name)
        self.shape = TexturedRectangle( texture_obj, self.size )
        self.shape.set_hotspot(Drawable.CENTER)
        #self.shape.thisown = 0
        self.node.set_drawable(self.shape)
        self.velocity = Vector2D(0.0, 0.0)
        self.last_velocity = None
        self.last_dt = 0.000001
        self.life = life
        self.max_life = life
        self.hit_sounds = ["hit1.wav", "hit2.wav", "hit3.wav", "hit4.wav"]
        self.life_hud = BarUI(self, "life", Color(1.0,0.0,0.0,1.0), Vector2D(0.0, self.radius))

    def Update(self, dt): ###
        self.UpdatePosition(dt)
        self.life_hud.Update()
        if self.velocity.Length() > 5000:
            self.velocity = self.velocity * (5000.0 / self.velocity.Length())

    def UpdatePosition(self, dt):
        pos = self.GetPos()
        pos = pos + (self.velocity * dt)
        self.last_velocity = self.velocity
        self.last_dt = dt
        self.HandleMapBoundaries(pos)
        self.node.modifier().set_offset(pos)

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
        #print self, "took %s damage, current life = %s" % (damage, self.life)

    def Heal(self, amount):
        if amount < 0:  return
        self.life += amount
        if self.life > self.max_life:
            self.life = self.max_life
        print self, "has recovered %s life, current life = %s" % (amount, self.life)
        
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
    dc = destination_range[1] - destination_range[0]    # dc = D - C
    r = dc * xa / ba                                    # r = (D-C)(X-A)/(B-A)
    return destination_range[0] + r       # return C + r
