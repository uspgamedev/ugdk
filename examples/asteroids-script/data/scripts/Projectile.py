from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity
from Animations import CreateExplosionFromCollision

class Projectile (BasicEntity):
    base_radius = 5.0
    @staticmethod
    def GetActualRadius(power):
        return Projectile.base_radius * power

    def __init__(self, x, y, velocity, power):
        self.power = power
        r = Projectile.GetActualRadius(power)
        BasicEntity.__init__(self, x, y, "images/projectile.png", r, 1)
        self.velocity = velocity
        self.damage = 25.0 * power
        self.lifetime = 10.0 * power
        self.life_hud.node.set_active(False)

    def Update(self, dt):
        self.UpdatePosition(dt)
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True

    def GetDamage(self, obj_type):
        if obj_type == "Planet.Planet":
            return self.damage * 0.05
        return self.damage

    def HandleCollision(self, target):
        if target.CheckType("Projectile"):
            # collision between projectiles, destroy both
            target.TakeDamage(666)
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, self.radius*5)
            #print "Projectiles exploding..."
        elif target.CheckType("Ship") or target.CheckType("Asteroid"):
            target.TakeDamage(self.GetDamage(target.type))
            target.ApplyVelocity(self.velocity * (0.15*self.power))
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, target.radius)
            #print "Projectile damaging ", target.type
        elif target.CheckType("Planet"):
            target.TakeDamage(self.GetDamage(target.type))
            self.is_destroyed = True
            CreateExplosionFromCollision(self, target, target.radius*0.7)
            #print "Projectile impacted planet"
