from ugdk.ugdk_base import Engine_reference
from ugdk.ugdk_math import Vector2D
from ugdk.ugdk_spritesheet import FlexibleSpritesheet
from ugdk.ugdk_drawable import Sprite
from BasicEntity import EntityInterface

def CreateSpritesheet(path, frame_width, frame_height, hotspot):
    resources = Engine_reference().resource_manager()
    tex = resources.texture_container().Load(path, path)
    
    sheet = FlexibleSpritesheet(tex)
    sheet.set_frame_size( Vector2D(frame_width, frame_height) )
    sheet.set_hotspot(hotspot)
    resources.spritesheet_container().Insert(path, sheet)


def InitializeSpritesheets():
    CreateSpritesheet("images/explosion.png", 128.0, 128.0, Vector2D(64.0,64.0) )


class AnimationEntity (EntityInterface):
    def __init__(self, x, y, sprite):
        EntityInterface.__init__(self, x, y, 1.0)
        self.sprite = sprite
        self.node.set_drawable(self.sprite)
        self.is_collidable = False
        
    def HandleCollision(self, target):
        pass
        
def CreateExplosionAtEntity(ent):
    animset = Engine_reference().resource_manager().animation_loader().Load("animations/explosion.gdd", "animations/explosion.gdd")
    sheet = Engine_reference().resource_manager().spritesheet_container().Find("images/explosion.png")
    sprite = Sprite(sheet, animset)
    sprite.SelectAnimation("BASIC_EXPLOSION")
    sprite_ent = AnimationEntity(ent.GetPos().get_x(), ent.GetPos().get_y(), sprite)
    ent.new_objects.append(sprite_ent)
