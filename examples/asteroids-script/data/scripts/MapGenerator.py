from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from Ship import Ship
from Asteroid import Asteroid
import random

MAX_ENTITY_SIZE = 100.0  #in pixels

PERCENT_OF_ENTITIES_IN_MAP_GRID = 0.075

ASTEROID_STARTING_SPEED_RANGE = [20, 90]

def GetCellCenter(i, j):
    x = j * MAX_ENTITY_SIZE + (MAX_ENTITY_SIZE/2.0)
    y = i * MAX_ENTITY_SIZE + (MAX_ENTITY_SIZE/2.0)
    return (x, y)

def Generate():
    screenSize = Engine_reference().video_manager().video_size()
    
    entities = []
    # for the purpose of generating random map objects, we create a imaginary table henceforth known as the "MAP"
    # the MAP represents the playable area, that is, the screen.
    # each cell in the map is of the size MAX_ENTITY_SIZExMAX_ENTITY_SIZE
    # a object can only be placed in the center position of a cell, and only one object per cell,
    # although there can be empty cells.
    #
    # this way we have a simple way of knowing where we can place object, with the benefit that
    # at least in theory, they should not be colliding with each other
    rows = int( screenSize.get_y() / MAX_ENTITY_SIZE )
    columns = int( screenSize.get_x() / MAX_ENTITY_SIZE )

    map_row = [False] * columns
    map = [map_row] * rows

    n = int( (rows * columns) * PERCENT_OF_ENTITIES_IN_MAP_GRID )

    print "Generating a %sx%s map with %s entities..." % (rows, columns, n)

    possibleCells = []
    for i in range(rows):
        for j in range(columns):
            possibleCells.append( (i,j) )
    random.shuffle(possibleCells)

    def pickAPlace():
        i, j = possibleCells.pop()
        map[i][j] = True
        return GetCellCenter(i, j)
    ##

    loc = pickAPlace()
    ship = Ship(loc[0], loc[1])
    entities.append(ship)

    for i in range(n):
        loc = pickAPlace()
        ast = Asteroid(loc[0], loc[1], random.random() + 0.5)
        v = Vector2D(random.random(), random.random())
        v = v.Normalize()
        v = v * (random.randint(ASTEROID_STARTING_SPEED_RANGE[0], ASTEROID_STARTING_SPEED_RANGE[1]))
        ast.ApplyVelocity(v)
        entities.append(ast)

    return entities