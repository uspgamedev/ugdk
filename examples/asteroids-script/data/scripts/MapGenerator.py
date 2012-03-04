from ugdk.ugdk_graphic import Node, Modifier
from ugdk.ugdk_drawable import SolidRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color, Engine_reference
import random

screenSize = None

class Shit:
    def __init__(self, x, y):
        self.size = Vector2D(50.0, 50.0)
        self.shape = SolidRectangle(self.size)
        self.shape.set_color( Color(0.0, 0.0, 1.0, 1.0) )
        self.node = Node( self.shape )
        self.node.modifier().set_offset( Vector2D(x,y) )
        self.elapsed = 0
    def Update(self, dt):
        self.elapsed += dt
        if self.elapsed > 2.0:
            self.elapsed = 0.0
            x = random.randint(0,800)
            y = random.randint(0,600)
            self.node.modifier().set_offset( Vector2D(x,y) )
        return


def Generate():
    screenSize = Engine_reference().video_manager().video_size()
    print screenSize, screenSize.get_x(), screenSize.get_y()
    entities = []
    for i in range(5):
        e = Shit(float(random.randint(0,800)), float(random.randint(0,600)) )
        entities.append(e)
    return entities