from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity, window_size
from Asteroid import Asteroid
from Gravity import GravityWell
from Shockwave import Shockwave
from random import random, randint, shuffle
from math import pi

from Projectile import Projectile

# yes, Planet is pretty similar to Asteroid... But, whatever =P
class Planet (BasicEntity):
    def __init__(self, x, y, size_factor):
        self.size_factor = size_factor
        r = 75.0 * size_factor
        hp = 6 * size_factor
        BasicEntity.__init__(self, x, y, "planet%s.png" % (randint(1,5)), r, hp)
        self.has_splitted = False
        self.well = GravityWell(x, y, r)
        self.well.AddIDToIgnoreList(self.id)
        self.new_objects.append(self.well)
        
    def Update(self, dt):
        BasicEntity.Update(self,dt)
        self.well.node.modifier().set_offset( self.GetPos() )

    def TakeDamage(self, damage):
        BasicEntity.TakeDamage(self, damage)
        # if we're big enough, split planet into asteroids when we are destroyed.
        if self.is_destroyed and not self.has_splitted:
            self.has_splitted = True
            self.well.is_destroyed = True #to assure our gravity well will be deleted with us
            # produce our shockwave before the asteroids since the C++ part pop()'s the objects
            # out of the list, so last objects in self.new_objects are created first.
            pos = self.GetPos()
            print "Planet cracking down..."
            wave = Shockwave(pos.get_x(), pos.get_y(), 4.0, [self.radius, window_size().Length() * 0.35])
            wave.AddIDToIgnoreList(self.id)
            self.new_objects.append(wave)
            print "Shockwave created"
            # and create our 'asteroid parts'
            angles = [0.0, -pi/4.0, -pi/2.0, -3*pi/2.0, pi, 3*pi/2.0, pi/2.0, pi/4.0]
            shuffle(angles)
            direction = Vector2D(1,0).Rotate(random()*2*pi)
            direction = direction.Normalize()
            factor = 0.75
            print self, "is breaking, into factor", factor
            for i in range(randint(2,5)):
                v = direction.Rotate(angles.pop())
                v = v * ((self.radius+Asteroid.GetActualRadius(factor))*1.15)
                pos = pos + v
                ast = Asteroid(pos.get_x(), pos.get_y(), factor)
                v = v.Normalize()
                speed = 20.0
                v = v * (randint(int(speed*0.60), int(speed*1.40)))
                ast.ApplyVelocity(v)
                self.new_objects.append(ast)

    def GetDamage(self, obj_type):
        return 9000.1 # Vegeta, what does the scouter say about his power level?

    def HandleCollision(self, target):
        if target.type == self.type:
            print "WTF dude, u tripping? Planets colliding with planets? Ya frakking nuts?"
            aux = self.velocity
            self.velocity = target.velocity
            target.velocity = aux
            self.ApplyCollisionRollback()
            target.ApplyCollisionRollback()
        elif target.type == "Ship.Ship":
            target.TakeDamage(self.GetDamage(target.type))
            #print target.type, "crash landed on Planet... No survivors.     Boo-hoo."
        # Projectiles and Asteroids take care of collising with Planets.

