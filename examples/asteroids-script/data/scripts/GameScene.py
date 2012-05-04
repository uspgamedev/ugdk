from ugdk.ugdk_action import Scene
from ugdk.ugdk_util import CreateBox2D
from ugdk.pyramidworks_collision import CollisionManager, CollisionInstanceList
from ugdk.ugdk_input import InputManager, K_ESCAPE, K_HOME, K_PAGEUP, K_PAGEDOWN
from ugdk.ugdk_base import Engine_reference, ResourceManager_CreateTextFromLanguageTag
from ugdk.ugdk_graphic import Node
from ugdk.ugdk_math import Vector2D
import Config
import MapGenerator
    
def StartupScene(df = 0.5):
    #print "STARTING SCENE"
    cena = AsteroidsScene(df)
    #print "GOING TO PUSH SCENE"
    Engine_reference().PushScene( cena )
    #print "GOING TO GENERATE MAP"
    cena.GenerateMap()
    #print "ALL DONE... Proceeding"
    print "=== Starting Asteroids Scene [Difficulty = %s]" % (df)
    return cena
    

class AsteroidsScene (Scene):
    def __init__(self, difficultyFactor):
        #print "Creating AsteroidsScene"
        maxval = MapGenerator.MAX_ENTITY_SIZE
        mincoords = [-maxval, -maxval]
        maxcoords = [Config.resolution.get_x() + maxval,  Config.resolution.get_y() + maxval]
        self.collisionManager = CollisionManager( CreateBox2D(mincoords[0], mincoords[1], maxcoords[0], maxcoords[1]) )
        self.objects = []
        self.colliding_objects = []
        self.startCollisions()
        self.asteroid_count = 0
        self.ship_alive = True
        self.hero = None
        self.finishTextNode = None
        self.difficultyFactor = difficultyFactor
        
    def startCollisions(self):
        self.collisionManager.Generate("Entity")
        self.collisionManager.Generate("Gravity")

    def GetHero(self):  return self.hero

    def Populate(self, objs):
        #print self, " POPULATE: receiving objects ", objs
        for obj in objs:
            self.AddObject(obj)
            
    def AddObject(self, obj):
        self.objects.append(obj)
        if obj.is_collidable:
            self.colliding_objects.append(obj)
            obj.collision_object.StartColliding()
        self.AddEntity(obj)
        #print self, "GOING TO ADD OBJECT %s [node=%s]" % (obj, obj.node)
        CN = self.content_node()
        CN.AddChild(obj.node)
        #print "SCENE CONTENT NODE = ", CN
        self.interface_node().AddChild(obj.hud_node)
        #print "FINISHED ADDING OBJECT"
        if obj.CheckType("Asteroid"):
            self.asteroid_count += 1
        if obj.CheckType("Ship"):
            self.ship_alive = True
            self.hero = obj
            
        
    def RemoveObject(self, obj):
        if obj.CheckType("Asteroid"):
            self.asteroid_count -= 1
        if obj.CheckType("Ship"):
            self.ship_alive = False
            self.hero = None
        self.objects.remove(obj)
        if obj in self.colliding_objects:
            self.colliding_objects.remove(obj)
            obj.collision_object.StopColliding()
        self.RemoveEntity(obj)
        #print "REMOVING OBJECT %s [%s]" % (obj, obj.node)
        del obj.node
        del obj.hud_node
        del obj
        
    def GenerateMap(self):
        #print "GENERATE MARK 1"
        self.Populate( MapGenerator.Generate(self.difficultyFactor) )
        #print "GENERATE MARK 2"
        self.content_node().set_drawable(MapGenerator.GetBackgroundDrawable() )
        #print "GENERATE MARK 3"
        
    def SetAndShowSceneEndText(self, msgTag):
        if self.finishTextNode != None: return
        text = ResourceManager_CreateTextFromLanguageTag(msgTag)
        self.finishTextNode = Node(text)
        screenSize = Engine_reference().video_manager().video_size()
        x = (screenSize.get_x()/2.0) - (text.width()/2.0)
        y = (screenSize.get_y()/2.0) - (text.height()/2.0)
        self.finishTextNode.modifier().set_offset( Vector2D(x, y) )
        self.interface_node().AddChild(self.finishTextNode)

    def Focus(self):
        pass

    def DeFocus(self):
        pass

    def Update(self, dt):  ###
        to_delete = []
        for obj in self.objects:
            if len(obj.new_objects) > 0:
                self.Populate(obj.new_objects)
                obj.new_objects = []
                
            if obj.is_destroyed:
                to_delete.append(obj)
            elif obj.is_collidable:
                obj.collision_object.MoveTo(obj.GetPos())
                
        for obj in to_delete:
            self.RemoveObject(obj)
            
        if self.asteroid_count <= 0:
            self.SetAndShowSceneEndText("GameWon")
        elif not self.ship_alive:
            self.SetAndShowSceneEndText("GameOver")

        self.CheckCommands()
        self.HandleCollisions()
        
    def CheckCommands(self):
        input = Engine_reference().input_manager()
        
        if input.KeyPressed(K_ESCAPE):
            print "GameScene ESCAPING"
            self.Finish()
        elif input.KeyPressed(K_PAGEUP):
            self.Finish()
            StartupScene(self.difficultyFactor * 1.15)
        elif input.KeyPressed(K_PAGEDOWN):
            self.Finish()
            StartupScene(self.difficultyFactor * 0.85)
        elif input.KeyPressed(K_HOME):
            self.Finish()
            StartupScene(self.difficultyFactor)
            
    def HandleCollisions(self):
        collision_list = CollisionInstanceList()
        #print "HandleCollisions COLLISION_LIST = ", collision_list
        for obj in self.colliding_objects:
            if not obj.is_destroyed:
                obj.collision_object.SearchCollisions(collision_list)
            
        for col in collision_list:
            #print "HC", col
            #print "HANDLE COLLISION::  [%s].Handle(%s)" % (col[0], col[1])
            col[0].Handle(col[1])
            
            
    def End(self):
        pass

