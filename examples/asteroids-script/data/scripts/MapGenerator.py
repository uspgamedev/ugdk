from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_base import Engine_reference, ResourceManager_GetTextureFromFile
from ugdk.ugdk_graphic import Node
from ugdk.ugdk_drawable import TexturedRectangle
from Ship import Ship
from Asteroid import Asteroid
from Planet import Planet
import random
from math import pi
import Config

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

def Generate(difficultyFactor, heroData):
    screenSize = Config.gamesize
    print "Screen Size = (%s, %s)" % (screenSize.get_x(), screenSize.get_y())
    
    entities = []
    # for the purpose of generating random map objects, we create a imaginary table henceforth known as the "MAP"
    # the MAP represents the playable area (from now on, no longer related to the screen).
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
    ship = Ship(loc[0], loc[1], heroData)
    entities.append(ship)

    planetCount = int( random.randint(PLANET_COUNT_RANGE[0], PLANET_COUNT_RANGE[1])   )
    for i in range(planetCount):
        loc = pickAPlace()
        planet = Planet(loc[0], loc[1], GetRandomPlanetSizeFactor())
        entities.append(planet)

    for i in range(n):
        loc = pickAPlace()
        ast = Asteroid(loc[0], loc[1], GetRandomAsteroidSizeFactor()*difficultyFactor)
        v = Vector2D(1,0).Rotate( random.random() * 2 * pi )
        v = v.Normalize()
        v = v * (random.randint(ASTEROID_STARTING_SPEED_RANGE[0], ASTEROID_STARTING_SPEED_RANGE[1])) * difficultyFactor
        ast.ApplyVelocity(v)
        entities.append(ast)

    return entities


def GetBackgroundDrawable(i):
    screenSize = Config.gamesize * 0.5 #Engine_reference().video_manager().video_size()
    texture_obj = ResourceManager_GetTextureFromFile("images/background%s.jpg" % (i))
    background = TexturedRectangle( texture_obj, screenSize )
    background.thisown = 0
    return background

def GetBackgroundSingleNode(i):
    fullnode = Node()

    topleft = Node(GetBackgroundDrawable(i))

    topright = Node(GetBackgroundDrawable(i))
    topright.modifier().set_scale(Vector2D(-1.0, 1.0))
    topright.modifier().set_offset(Vector2D(Config.gamesize.get_x(), 0.0))

    botleft = Node(GetBackgroundDrawable(i))
    botleft.modifier().set_scale(Vector2D(1.0, -1.0))
    botleft.modifier().set_offset(Vector2D(0.0, Config.gamesize.get_y()))

    botright = Node(GetBackgroundDrawable(i))
    botright.modifier().set_scale(Vector2D(-1.0, -1.0))
    botright.modifier().set_offset(Config.gamesize)

    #fullnode.AddChild(topleft)
    fullnode.AddChild(topright)
    fullnode.AddChild(botleft)
    #fullnode.AddChild(botright)

    return fullnode

def GetBackgroundNode():
    bgnode = Node()
    r = random.randint(1,3)
    for i in range(2):
        for j in range(2):
            node = Node()
            node.AddChild(GetBackgroundSingleNode(r))
            node.modifier().set_offset(Vector2D(Config.gamesize.get_x() * i, Config.gamesize.get_y() * j))
            bgnode.AddChild(node)
    bgnode.set_zindex(-10000)
    return bgnode
