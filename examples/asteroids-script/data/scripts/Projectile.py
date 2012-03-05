from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity

class Projectile (BasicEntity):
    def __init__(self, x, y, velocity, power):
        self.power = power
        r = 5.0 * power
        BasicEntity.__init__(self, x, y, "projectile.png", r, 1)
        self.velocity = velocity
        self.damage = 20.0 * power
        self.lifetime = 10.0 * power

    def Update(self, dt):
        self.UpdatePosition(dt)
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True

    def GetDamage(self, obj_type):
        return self.damage

    def HandleCollision(self, target):
        if target.type == self.type:
            # collision between projectiles, destroy both
            target.TakeDamage(666)
            self.is_destroyed = True
            print "Projectiles exploding..."
        elif target.type == "Ship.Ship" or target.type == "Asteroid.Asteroid":
            target.TakeDamage(self.GetDamage(target.type))
            target.ApplyVelocity(self.velocity * (0.15*self.power))
            self.is_destroyed = True
            print "Projectile damaging ", target.type
        elif target.type == "Planet.Planet":
            self.TakeDamage(666)
            print "Projectile impacted planet"
