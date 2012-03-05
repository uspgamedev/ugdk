from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity
from random import randint, shuffle
from MapGenerator import ASTEROID_STARTING_SPEED_RANGE
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
		if self.is_destroyed and self.size_factor > 0.6:
			angles = [0.0, -pi/4.0, -pi/2.0, -3*pi/2.0, pi, 3*pi/2.0, pi/2.0, pi/4.0]
			shuffle(angles)
			direction = self.velocity.Normalize()
			#left = mouse_dir.Rotate(-pi/2.0)
			for i in range(randint(2,4)):
				factor = self.size_factor / 1.25
				v = direction.Rotate(angles.pop())
				v = v * (self.radius*0.9)
				pos = self.GetPos() + v
				ast = Asteroid(pos.get_x(), pos.get_y(), factor)
				v = v.Normalize()
				v = v * (randint(ASTEROID_STARTING_SPEED_RANGE[0], ASTEROID_STARTING_SPEED_RANGE[1]))
				ast.ApplyVelocity(v)
				self.new_objects.append(ast)
		
    def HandleCollision(self, target):
        pass
