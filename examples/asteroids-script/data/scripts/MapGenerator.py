from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_drawable import TexturedRectangle
from Ship import Ship
from Asteroid import Asteroid
from Planet import Planet
import random

MAX_ENTITY_SIZE = 200.0  #in pixels

PERCENT_OF_ENTITIES_IN_MAP_GRID = 0.3

ASTEROID_STARTING_SPEED_RANGE = [20, 90]

PLANET_COUNT_RANGE = [0, 2]

def GetCellCenter(i, j):
    x = j * MAX_ENTITY_SIZE + (MAX_ENTITY_SIZE/2.0)
    y = i * MAX_ENTITY_SIZE + (MAX_ENTITY_SIZE/2.0)
    return (x, y)
    
def GetRandomAsteroidSizeFactor():
	# returns random float in [0.5, 1.2[
	return random.random() * 0.7 + 0.5
	
def GetRandomPlanetSizeFactor():
	# returns random float in [0.7, 1.3[
	return random.random() * 0.6  + 0.7

def Generate():
    screenSize = Engine_reference().video_manager().video_size()
    print "Screen Size = (%s, %s)" % (screenSize.get_x(), screenSize.get_y())
    
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

    # WHAT SORCERY IS THIS?!?
    map_row = [False] * columns
    map = [map_row] * rows

    n = int( (rows * columns) * PERCENT_OF_ENTITIES_IN_MAP_GRID )

    print "Generating a %sx%s map with %s entities..." % (rows, columns, n)

    #possibleCells = []
    #for i in range(rows):
    #    for j in range(columns):
    #        possibleCells.append( (i,j) )
    possibleCells = [(i,j) for i in range(rows) for j in range(columns)]
    random.shuffle(possibleCells)
    # I had so many awesome ideas using awesome python features to do this 
    # (generate a list of all possible cell (x, y) locations)

    def pickAPlace():
        if len(possibleCells) == 0: return (random.random()*600, random.random()*400)
        i, j = possibleCells.pop()
        map[i][j] = True
        return GetCellCenter(i, j)
    ##

    loc = pickAPlace()
    ship = Ship(loc[0], loc[1])
    entities.append(ship)

    for i in range( random.randint(PLANET_COUNT_RANGE[0], PLANET_COUNT_RANGE[1]) ):
        loc = pickAPlace()
        planet = Planet(loc[0], loc[1], GetRandomPlanetSizeFactor())
        entities.append(planet)

    for i in range(n):
        loc = pickAPlace()
        ast = Asteroid(loc[0], loc[1], GetRandomAsteroidSizeFactor())
        v = Vector2D(random.random(), random.random())
        v = v.Normalize()
        v = v * (random.randint(ASTEROID_STARTING_SPEED_RANGE[0], ASTEROID_STARTING_SPEED_RANGE[1]))
        ast.ApplyVelocity(v)
        entities.append(ast)

    return entities


def GetBackgroundDrawable():
    screenSize = Engine_reference().video_manager().video_size()
    texture_obj = Engine_reference().resource_manager().texture_container().Load("background%s.jpg" % (random.randint(1,3)) )
    background = TexturedRectangle( texture_obj, screenSize )
    #background.set_hotspot(Drawable.CENTER)
    background.thisown = 0
    return background
    
