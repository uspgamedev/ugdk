from ugdk.ugdk_graphic import Node, Modifier
from ugdk.ugdk_drawable import SolidRectangle
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Color

class Shit:
    def __init__(self, x, y):
        self.size = Vector2D(50.0, 50.0)
        self.shape = SolidRectangle(self.size)
        self.shape.set_color( Color(0.0, 0.0, 1.0, 1.0) )
        self.Node = Node( self.shape, Modifier(Vector2D(x,y)) )
    def Update(self, dt):
        pass


def Generate():
    entities = []
    for i in range(5):
        e = Shit(i*100.0, i*100.0)
        entities.append(e)
    return entities