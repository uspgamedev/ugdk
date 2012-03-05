from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity
from random import randint, shuffle
from math import pi

class Asteroid (BasicEntity):
    def __init__(self, x, y, size_factor):
        self.size_factor = size_factor
        r = 55.0 * size_factor
        hp = 120 * size_factor
        BasicEntity.__init__(self, x, y, "asteroid.png", r, hp)
        
    def TakeDamage(self, damage):
        BasicEntity.TakeDamage(self, damage)
        # if we're big enough, split asteroid when we are destroyed.
        #if self.is_destroyed and self.size_factor > 0.6:
        #    angles = [0.0, -pi/4.0, -pi/2.0, -3*pi/2.0, pi, 3*pi/2.0, pi/2.0, pi/4.0]
        #    shuffle(angles)
        #    direction = self.velocity.Normalize()
        #    #left = mouse_dir.Rotate(-pi/2.0)
        #    for i in range(randint(2,4)):
        #        factor = self.size_factor / 1.25
        #        v = direction.Rotate(angles.pop())
        #        v = v * (self.radius*1.5)
        #        pos = self.GetPos() + v
        #        ast = Asteroid(pos.get_x(), pos.get_y(), factor)
        #        v = v.Normalize()
        #        speed = self.velocity.Length()
        #        v = v * (randint(int(speed*0.60), int(speed*1.40)))
        #        ast.ApplyVelocity(v)
        #        self.new_objects.append(ast)
		
    def GetDamage(self, obj_type):
        if obj_type == self.type:
            return self.life * 0.2
        elif obj_type == "Ship.Ship":
            return self.life

    def HandleCollision(self, target):
        if target.type == self.type:
            aux = self.velocity
            self.velocity = target.velocity
            target.velocity = aux
            self.ApplyCollisionRollback()
            target.ApplyCollisionRollback()
            self.TakeDamage(target.GetDamage(self.type))
            target.TakeDamage(self.GetDamage(target.type))
            print "Asteroid collided with asteroid"
        elif target.type == "Ship.Ship":
            target.TakeDamage(self.GetDamage(target.type))
            target.ApplyVelocity(self.velocity * 0.5)
            self.TakeDamage(self.life + 10) #just to make sure we die LOL
            print "Asteroid damaging ", target.type
        elif target.type == "Planet.Planet":
            pass

        #No handler for projectile since that is strictly
        #"do it only one time", and Projectile will handle it
