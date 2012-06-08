from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_spritesheet import FlexibleSpritesheet
from ugdk.ugdk_drawable import Sprite
from ugdk.ugdk_action import Observer
from BasicEntity import EntityInterface
from random import random
from math import pi

def CreateSpritesheet(path, frame_width, frame_height, hotspot):
    resources = Engine_reference().resource_manager()
    tex = resources.texture_container().Load(path, path)
    
    sheet = FlexibleSpritesheet(tex)
    sheet.set_frame_size( Vector2D(frame_width, frame_height) )
    sheet.set_hotspot(hotspot)
    resources.spritesheet_container().Insert(path, sheet)


def InitializeSpritesheets():
    CreateSpritesheet("images/explosion.png", 128.0, 128.0, Vector2D(64.0,64.0) )


class EndObserver (Observer):
    def __init__(self, animent):
        self.animent = animent
    def Tick(self):
        if self.animent != None:
            self.animent.is_destroyed = True
            self.animent = None
        
class AnimationEntity (EntityInterface):
    def __init__(self, x, y, sprite, radius):
        EntityInterface.__init__(self, x, y, radius)
        self.sprite = sprite
        self.observer = EndObserver( self)
        self.sprite.AddObserverToAnimation(self.observer)
        self.node.set_drawable(self.sprite)
        # sprite.size.x -> 1.0
        # radius -> SCALE
        scaleX = radius / sprite.size().get_x()
        scaleY = radius / sprite.size().get_y()
        self.node.modifier().set_scale( Vector2D(scaleX, scaleY) )
        self.node.modifier().set_rotation( random() * 2 * pi )
        self.is_collidable = False
        
    def HandleCollision(self, target):
        pass
        
def CreateExplosionFromCollision(ent, target, radius):
    animset = Engine_reference().resource_manager().animation_loader().Load("animations/explosion.gdd", "animations/explosion.gdd")
    sheet = Engine_reference().resource_manager().spritesheet_container().Find("images/explosion.png")
    sprite = Sprite(sheet, animset)
    sprite.SelectAnimation("BASIC_EXPLOSION")
    pos = ent.GetPos()
    v = target.GetPos() - pos
    v = v.Normalize()
    pos = pos + (v * ent.radius)
    posX = pos.get_x()
    posY = pos.get_y()
    sprite_ent = AnimationEntity(posX, posY, sprite, radius)
    ent.new_objects.append(sprite_ent)
    return sprite_ent

def CreateExplosionAtLocation(pos, radius):
    animset = Engine_reference().resource_manager().animation_loader().Load("animations/explosion.gdd", "animations/explosion.gdd")
    sheet = Engine_reference().resource_manager().spritesheet_container().Find("images/explosion.png")
    sprite = Sprite(sheet, animset)
    sprite.SelectAnimation("BASIC_EXPLOSION")
    posX = pos.get_x()
    posY = pos.get_y()
    sprite_ent = AnimationEntity(posX, posY, sprite, radius)
    return sprite_ent