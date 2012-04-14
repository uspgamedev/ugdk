from ugdk.ugdk_action import Scene
from ugdk.ugdk_util import CreateBox2D
from ugdk.pyramidworks_collision import CollisionManager, CollisionInstanceList
from ugdk.ugdk_input import InputManager, K_ESCAPE, K_HOME
from ugdk.ugdk_base import Engine_reference
import Config
import MapGenerator
    
def StartupScene():
    print "STARTING SCENE"
    cena = AsteroidsScene()
    print "GOING TO PUSH SCENE"
    Engine_reference().PushScene( cena )
    print "GOING TO GENERATE MAP"
    cena.GenerateMap()
    print "ALL DONE... Proceeding"
    return cena
    

class AsteroidsScene (Scene):
    def __init__(self):
        print "Creating AsteroidsScene"
        #Scene.__init__(self)
        print "Initialized base class"
        maxval = MapGenerator.MAX_ENTITY_SIZE
        mincoords = [-maxval, -maxval]
        maxcoords = [Config.resolution.get_x() + maxval,  Config.resolution.get_y() + maxval]
        self.collisionManager = CollisionManager( CreateBox2D(mincoords[0], mincoords[1], maxcoords[0], maxcoords[1]) )
        self.objects = []
        self.colliding_objects = []
        self.startCollisions()
        #self.thisown = 0
        
    def startCollisions(self):
        self.collisionManager.Generate("Entity")
        self.collisionManager.Generate("Gravity")

    def Populate(self, objs):
        print self, " POPULATE: receiving objects ", objs
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
            
        
    def RemoveObject(self, obj):
        self.objects.remove(obj)
        if obj in self.colliding_objects:
            self.colliding_objects.remove(obj)
            obj.collision_object.StopColliding()
        self.RemoveEntity(obj)
        print "REMOVING OBJECT %s [%s]" % (obj, obj.node)
        #obj.node.__swig_destroy__(obj.node)
        del obj.node
        del obj.hud_node
        del obj
        
    def GenerateMap(self):
        self.Populate( MapGenerator.Generate() )
        self.content_node().set_drawable(MapGenerator.GetBackgroundDrawable() )
        
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
            
        self.CheckCommands()
        self.HandleCollisions()
        
    def CheckCommands(self):
        input = Engine_reference().input_manager()
        
        if input.KeyPressed(K_ESCAPE):
            print "GameScene ESCAPING"
            self.Finish()
        elif input.KeyPressed(K_HOME):
            self.Finish()
            StartupScene()
            
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

