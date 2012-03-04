from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from Ship import Ship
import random

def Generate():
    screenSize = Engine_reference().video_manager().video_size()
    
    entities = []
    #for i in range(5):
    #    e = WATWATWAT(float(random.randint(0,800)), float(random.randint(0,600)) )
    #    entities.append(e)
    e = Ship(float(random.randint(0,800)), float(random.randint(0,600)) )
    entities.append(e)
    return entities