from ugdk.ugdk_action import Scene, Task
from ugdk.ugdk_util import CreateBox2D
from ugdk.pyramidworks_collision import CollisionManager, CollisionInstanceList
from ugdk.ugdk_input import InputManager, K_ESCAPE, K_HOME, K_PAGEUP, K_PAGEDOWN, K_p, K_SPACE
from ugdk.ugdk_base import Engine_reference, ResourceManager_CreateTextFromLanguageTag, Color
from ugdk.ugdk_drawable import SolidRectangle
from ugdk.ugdk_graphic import Node
from ugdk.ugdk_math import Vector2D
import Config
import MapGenerator
import BarUI
import Ship
    

import ItemFactory

def StartupScene():
    #print "STARTING SCENE"
    cena = ManagerScene()
    #print "GOING TO PUSH SCENE"
    Engine_reference().PushScene( cena )
    return cena
    

class ManagerScene (Scene):
    IDLE = 0
    ACTIVE = 1
    PLAYER_DIED = 2
    PLAYER_WON = 3
    PLAYER_QUIT = 4
    def __init__(self):
        self.lives = 5
        self.difficulty = 0.5
        self.points = 0
        self.status = ManagerScene.IDLE
        self.heroData = Ship.ShipData(100.0, 100.0, 25.0)
        self.stats = BarUI.StatsUI(self, 100.0, 100.0, Color(0.6,0.6,0.6), 0.4 )
        self.interface_node().AddChild(self.stats.node)

    def Focus(self):
        self.stats.node.set_active(True)

    def DeFocus(self):
        self.stats.node.set_active(False)

    def Update(self, dt):  ###
        self.CheckCommands()
        self.stats.Update()
        if self.status == ManagerScene.PLAYER_QUIT:
            self.Finish()
            return
        replay = self.status != ManagerScene.ACTIVE
        if replay and self.lives > 0:
            if self.status == ManagerScene.PLAYER_WON:
                self.difficulty *= 1.15
                print "Game WON!"
            elif self.status == ManagerScene.PLAYER_DIED:
                print "You are no match for teh might of teh Asteroid Army!"
            cena = AsteroidsScene(self, self.difficulty)
            Engine_reference().PushScene(cena)
            cena.GenerateMap(self.heroData)
            self.status = ManagerScene.ACTIVE
            print "=== Starting Asteroids Scene [Difficulty = %s][Lives Left = %s][%s]" % (self.difficulty, self.lives, self.heroData)
            
    def UpdateLives(self, amount): self.lives += amount
    def UpdatePoints(self, amount): self.points += amount

    def SetGameResult(self, won):
        if won:
            self.status = ManagerScene.PLAYER_WON
        else:
            self.status = ManagerScene.PLAYER_DIED
            self.UpdateLives(-1)
    def SetGameQuit(self):
        self.status = ManagerScene.PLAYER_QUIT

    def CheckCommands(self):
        input = Engine_reference().input_manager()
        
        if input.KeyPressed(K_ESCAPE):
            print "Manager ESCAPING"
            self.Finish()

    def End(self):
        pass

class SceneFinishTask(Task):
    def __init__(self, delay):
        self.timeRemaining = delay

    def __call__(self, dt):
        self.timeRemaining -= dt
        if self.timeRemaining <= 0:
            Engine_reference().CurrentScene().Finish()
            return False
        return True


class AsteroidsScene (Scene):
    def __init__(self, managerScene, difficultyFactor):
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
        #self.AddTask(self.collisionManager.GenerateHandleCollisionTask() )
        self.managerScene = managerScene
        self.stats = BarUI.StatsUI(managerScene, 0.0, 0.0, Color(0.0,0.0,0.0), 0.4 )
        
    def startCollisions(self):
        self.collisionManager.Generate("Entity")
        self.collisionManager.Generate("Gravity")
        self.collisionManager.Generate("PowerUp")
        self.collisionManager.Generate("RangeCheck")

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
            self.managerScene.heroData = self.hero.data
            self.hero = None
        self.managerScene.UpdatePoints( obj.GetPointsValue() )
        self.objects.remove(obj)
        if obj in self.colliding_objects:
            self.colliding_objects.remove(obj)
            obj.collision_object.StopColliding()
        self.RemoveEntity(obj)
        #print "REMOVING OBJECT %s [%s]" % (obj, obj.node)
        obj.node.thisown = 1
        del obj.node
        obj.hud_node.thisown = 1
        del obj.hud_node
        del obj
        
    def GenerateMap(self, heroData):
        #print "GENERATE MARK 1"
        self.Populate( MapGenerator.Generate(self.difficultyFactor, heroData) )
        #print "GENERATE MARK 2"
        self.content_node().set_drawable(MapGenerator.GetBackgroundDrawable() )
        #print "GENERATE MARK 3"
        self.interface_node().AddChild(self.stats.node)
        
    def GetLivePlanetsPoints(self):
        v = 0
        for obj in self.objects:
            if obj.CheckType("Planet") and not obj.is_destroyed:
                v += obj.life
        return v

    def SetAndShowSceneEndText(self, msgTag):
        if self.finishTextNode != None: return
        text = ResourceManager_CreateTextFromLanguageTag(msgTag)
        self.finishTextNode = Node(text)
        screenSize = Engine_reference().video_manager().video_size()
        x = (screenSize.get_x()/2.0) - (text.width()/2.0)
        y = (screenSize.get_y()/2.0) - (text.height()/2.0)
        self.finishTextNode.modifier().set_offset( Vector2D(x, y) )
        self.interface_node().AddChild(self.finishTextNode)

    def CheckForEndGame(self):
        if self.finishTextNode != None: return
        if self.asteroid_count <= 0:
            self.SetAndShowSceneEndText("GameWon")
            self.managerScene.SetGameResult(True)
            self.managerScene.UpdatePoints( self.GetLivePlanetsPoints() )
            phl = 100
            if self.hero != None:
                phl = self.hero.life * 5
            self.managerScene.UpdatePoints( phl )
            self.AddTask(SceneFinishTask(5.0))
        elif not self.ship_alive:
            self.SetAndShowSceneEndText("GameOver")
            self.managerScene.SetGameResult(False)
            self.AddTask(SceneFinishTask(5.0))
        
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
            
        self.CheckForEndGame()
        self.stats.Update()
        self.CheckCommands()
        self.HandleCollisions()

        if self.hero != None:
            video_size = Engine_reference().video_manager().video_size()
            self.content_node().modifier().set_offset(-self.hero.GetPos() + video_size * 0.5)
        
    def CheckCommands(self):
        input = Engine_reference().input_manager()
        
        if input.KeyPressed(K_ESCAPE):
            print "GameScene ESCAPING"
            self.managerScene.SetGameQuit()
            self.Finish()
        elif input.KeyPressed(K_SPACE) or input.KeyPressed(K_p):
            Engine_reference().PushScene( PauseScene() )
        ### cheats
        elif input.KeyPressed(K_PAGEUP):
            self.managerScene.difficulty *= 1.15
        elif input.KeyPressed(K_PAGEDOWN):
            self.managerScene.difficulty *= 0.85
        elif input.KeyPressed(K_HOME):
            self.managerScene.lives += 1
            cheat = ItemFactory.CreatePassivePack(self.hero.GetPos().get_x(), self.hero.GetPos().get_y())
            self.AddObject(cheat)
        
            
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
        if self.hero != None:
            self.managerScene.heroData = self.hero.data

################################################################################

class PauseScene (Scene):
    def __init__(self):
        screenSize = Engine_reference().video_manager().video_size()
        rect = SolidRectangle(screenSize)
        rect.set_color( Color(0.5, 0.5, 0.5) )
        self.backNode = Node(rect)
        self.backNode.modifier().set_alpha(0.5)
        self.content_node().AddChild(self.backNode)

        text = ResourceManager_CreateTextFromLanguageTag("Pause")
        self.textNode = Node(text)
        x = (screenSize.get_x()/2.0) - (text.width()/2.0)
        y = (screenSize.get_y()/2.0) - (text.height()/2.0)
        self.textNode.modifier().set_offset( Vector2D(x, y) )
        self.interface_node().AddChild(self.textNode)

    def Focus(self):
        pass

    def DeFocus(self):
        pass

    def Update(self, dt):  ###
        input = Engine_reference().input_manager()
        resume = False
        resume = resume or input.KeyPressed(K_ESCAPE)
        resume = resume or input.KeyPressed(K_HOME)
        resume = resume or input.KeyPressed(K_PAGEUP)
        resume = resume or input.KeyPressed(K_PAGEDOWN)
        resume = resume or input.KeyPressed(K_p)
        resume = resume or input.KeyPressed(K_SPACE)
        if resume:
            self.Finish()

            
    def End(self):
        pass
