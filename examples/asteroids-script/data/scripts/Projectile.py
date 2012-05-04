from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from BasicEntity import BasicEntity
from Animations import CreateExplosionFromCollision

class Projectile (BasicEntity):
    base_radius = 5.0
    @staticmethod
    def GetActualRadius(power):
        return Projectile.base_radius * power

    def __init__(self, x, y, velocity, power):
        self.power = power
        self.damage = 25.0 * power
        self.original_damage = self.damage
        self.lifetime = 10.0 * power
        self.original_radius = Projectile.GetActualRadius(power)
        BasicEntity.__init__(self, x, y, "images/projectile.png", self.original_radius, self.damage)
        self.velocity = velocity
        self.life_hud.node.set_active(False)

    def Update(self, dt):
        self.UpdatePosition(dt)
        self.lifetime -= dt
        if self.lifetime <= 0:
            #gotta destroy this thing
            self.is_destroyed = True

    def TakeDamage(self, damage):
        BasicEntity.TakeDamage(self, damage)
        scale = self.life / self.original_damage
        self.node.modifier().set_scale( Vector2D(scale, scale) )

    def GetDamage(self, obj_type):
        if obj_type == "Planet":
            return self.damage * 0.05
        return self.damage

    def HandleCollision(self, target):
        if target.CheckType("Projectile"):
            # collision between projectiles, destroy both
            target.TakeDamage(self.GetDamage(target.type))
            #self.is_destroyed = True
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


#########################
class Turret:
    def __init__(self, parent, cooldown, speed, power):
        self.parent = parent
        self.cooldown = cooldown
        self.elapsed = 0.0
        self.speed = speed
        self.power = power
    
    def Update(self, dt):
        self.elapsed += dt
        if self.elapsed > self.cooldown:
            self.elapsed = 0.0
            target = self.GetTarget()
            if target != None:
                self.Shoot( target )

    def GetTarget(self):
        return Engine_reference().CurrentScene().GetHero()

    def Shoot(self, target):
        pos = self.parent.GetPos()
        dir = target.GetPos() - pos
        dir = dir.Normalize()
        dir = dir * 1.1 * (self.parent.radius + Projectile.GetActualRadius(self.power))
        pos = pos + dir
        vel = self.parent.velocity + (dir.Normalize() * self.speed)
        proj = Projectile(pos.get_x(), pos.get_y(), vel, self.power)
        self.parent.new_objects.append(proj)
        if hasattr(target, "radio"): #yay pog
            target.radio.PlaySound("fire.wav")


###################################

class Shield(BasicEntity):
    def __init__(self, parent):
        self.parent = parent
        self.active = False
        BasicEntity.__init__(self, x, y, "images/shockwave.png", parent.radius*1.1, 1)


from Gravity import GravityWell

class AntiGravShield(GravityWell):
    def __init__(self, parent, energyCostPerSec):
        self.parent = parent
        self.energyCostPerSec = energyCostPerSec
        GravityWell.__init__(self, parent.GetPos().get_x(), parent.GetPos().get_y(), parent.radius*3)
        self.mass *= 10
        self.AddIDToIgnoreList(parent.id)
        self.is_antigrav = True
        parent.new_objects.append(self)
    
    def Update(self, dt):
        if self.parent.is_destroyed:
            self.is_destroyed = True
        else:
            GravityWell.Update(self, dt)
            self.node.modifier().set_offset( self.parent.GetPos() )
            if self.active and hasattr(self.parent, "energy"):
                if self.parent.energy < self.parent.max_energy*0.05:
                    self.active = False
                else:
                    self.parent.energy -= self.energyCostPerSec * dt
