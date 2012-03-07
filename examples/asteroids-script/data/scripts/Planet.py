from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity
from Asteroid import Asteroid
from Gravity import GravityWell
from random import random, randint, shuffle
from math import pi

# yes, Planet is pretty similar to Asteroid... But, whatever =P
class Planet (BasicEntity):
    def __init__(self, x, y, size_factor):
        self.size_factor = size_factor
        r = 75.0 * size_factor
        hp = 600 * size_factor
        BasicEntity.__init__(self, x, y, "planet.png", r, hp)
        self.has_splitted = False
        self.well = GravityWell(x, y, r)
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
            angles = [0.0, -pi/4.0, -pi/2.0, -3*pi/2.0, pi, 3*pi/2.0, pi/2.0, pi/4.0]
            shuffle(angles)
            direction = Vector2D(random(), random())
            direction = direction.Normalize()
            factor = 0.75
            print self, "is breaking, into factor", factor
            for i in range(randint(2,5)):
                v = direction.Rotate(angles.pop())
                v = v * ((self.radius+Asteroid.GetActualRadius(factor))*1.15)
                pos = self.GetPos() + v
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
            print target.type, "crash landed on Planet... No survivors.     Boo-hoo."
        # Projectiles and Asteroids take care of collising with Planets.

