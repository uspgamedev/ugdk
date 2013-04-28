SET(UGDK_SRC 
    ./src/pyramidworks/collision.h 
    ./src/pyramidworks/collision/collisionclass.cc 
    ./src/pyramidworks/collision/collisionclass.h 
    ./src/pyramidworks/collision/collisionlogic.h 
    ./src/pyramidworks/collision/collisionmanager.cc 
    ./src/pyramidworks/collision/collisionmanager.h 
    ./src/pyramidworks/collision/collisionobject.cc 
    ./src/pyramidworks/collision/collisionobject.h 
    ./src/pyramidworks/collision/genericcollisionlogic.h 
    ./src/pyramidworks/geometry.h 
    ./src/pyramidworks/geometry/circle.cc 
    ./src/pyramidworks/geometry/circle.h 
    ./src/pyramidworks/geometry/convexpolygon.cc 
    ./src/pyramidworks/geometry/convexpolygon.h 
    ./src/pyramidworks/geometry/geometricshape.h 
    ./src/pyramidworks/geometry/rect.cc 
    ./src/pyramidworks/geometry/rect.h 
    ./src/ugdk/action.h 
    ./src/ugdk/action/animationplayer.h 
    ./src/ugdk/action/entity.h 
    ./src/ugdk/action/generictask.h 
    ./src/ugdk/action/mediaplayer.cc 
    ./src/ugdk/action/mediaplayer.h 
    ./src/ugdk/action/observer.h 
    ./src/ugdk/action/scene.cc 
    ./src/ugdk/action/scene.h 
    ./src/ugdk/action/spriteanimationframe.h 
    ./src/ugdk/action/spritetypes.cc 
    ./src/ugdk/action/spritetypes.h 
    ./src/ugdk/action/task.h 
    ./src/ugdk/audio.h 
    ./src/ugdk/audio/audiomanager.cc 
    ./src/ugdk/audio/audiomanager.h 
    ./src/ugdk/audio/music.cc 
    ./src/ugdk/audio/music.h 
    ./src/ugdk/audio/sample.cc 
    ./src/ugdk/audio/sample.h 
    ./src/ugdk/base/configuration.h 
    ./src/ugdk/base/engine.cc 
    ./src/ugdk/base/engine.h 
    ./src/ugdk/base/genericcontainer.h 
    ./src/ugdk/base/resourcecontainer.h 
    ./src/ugdk/base/resourcemanager.cc 
    ./src/ugdk/base/resourcemanager.h 
    ./src/ugdk/base/types.h 
    ./src/ugdk/graphic.h 
    ./src/ugdk/graphic/defaultshaders.cc 
    ./src/ugdk/graphic/defaultshaders.h 
    ./src/ugdk/graphic/drawable.h 
    ./src/ugdk/graphic/drawable/label.cc 
    ./src/ugdk/graphic/drawable/label.h 
    ./src/ugdk/graphic/drawable/sprite.cc 
    ./src/ugdk/graphic/drawable/sprite.h 
    ./src/ugdk/graphic/drawable/textbox.cc 
    ./src/ugdk/graphic/drawable/textbox.h 
    ./src/ugdk/graphic/drawable/texturedrectangle.cc 
    ./src/ugdk/graphic/drawable/texturedrectangle.h 
    ./src/ugdk/graphic/font.cc 
    ./src/ugdk/graphic/font.h 
    ./src/ugdk/graphic/geometry.cc 
    ./src/ugdk/graphic/geometry.h 
    ./src/ugdk/graphic/light.cc 
    ./src/ugdk/graphic/light.h 
    ./src/ugdk/graphic/node.cc 
    ./src/ugdk/graphic/node.h 
    ./src/ugdk/graphic/opengl/Exception.cc 
    ./src/ugdk/graphic/opengl/Exception.h 
    ./src/ugdk/graphic/opengl/shader.cc 
    ./src/ugdk/graphic/opengl/shader.h 
    ./src/ugdk/graphic/opengl/shaderprogram.cc 
    ./src/ugdk/graphic/opengl/shaderprogram.h 
    ./src/ugdk/graphic/opengl/vertexbuffer.cc 
    ./src/ugdk/graphic/opengl/vertexbuffer.h 
    ./src/ugdk/graphic/spritesheet.cc 
    ./src/ugdk/graphic/spritesheet.h 
    ./src/ugdk/graphic/textmanager.cc 
    ./src/ugdk/graphic/textmanager.h 
    ./src/ugdk/graphic/texture.cc 
    ./src/ugdk/graphic/texture.h 
    ./src/ugdk/graphic/videomanager.cc 
    ./src/ugdk/graphic/videomanager.h 
    ./src/ugdk/graphic/visualeffect.h 
    ./src/ugdk/input.h 
    ./src/ugdk/input/inputmanager.cc 
    ./src/ugdk/input/inputmanager.h 
    ./src/ugdk/input/keys.h 
    ./src/ugdk/math.h 
    ./src/ugdk/math/frame.cc 
    ./src/ugdk/math/frame.h 
    ./src/ugdk/math/integer2D.cc 
    ./src/ugdk/math/integer2D.h 
    ./src/ugdk/math/vector2D.cc 
    ./src/ugdk/math/vector2D.h 
    ./src/ugdk/portable/tr1.h 
    ./src/ugdk/script.h 
    ./src/ugdk/script/baseproxy.h 
    ./src/ugdk/script/defs.h 
    ./src/ugdk/script/initscripts.cc 
    ./src/ugdk/script/langwrapper.h 
    ./src/ugdk/script/module.h 
    ./src/ugdk/script/scriptmanager.cc 
    ./src/ugdk/script/scriptmanager.h 
    ./src/ugdk/script/type.h 
    ./src/ugdk/script/virtualdata.h 
    ./src/ugdk/script/virtualobj.cc 
    ./src/ugdk/script/virtualobj.h 
    ./src/ugdk/script/virtualprimitive.h 
    ./src/ugdk/time.h 
    ./src/ugdk/time/timeaccumulator.cc 
    ./src/ugdk/time/timeaccumulator.h 
    ./src/ugdk/time/timemanager.cc 
    ./src/ugdk/time/timemanager.h 
    ./src/ugdk/ui.h 
    ./src/ugdk/ui/button.cc 
    ./src/ugdk/ui/button.h 
    ./src/ugdk/ui/menu.cc 
    ./src/ugdk/ui/menu.h 
    ./src/ugdk/ui/uielement.cc 
    ./src/ugdk/ui/uielement.h 
    ./src/ugdk/util.h 
    ./src/ugdk/util/animationprotocol.cc 
    ./src/ugdk/util/animationprotocol.h 
    ./src/ugdk/util/gdd/descriptionprotocol.h 
    ./src/ugdk/util/gdd/descriptionprotocolbase.cc 
    ./src/ugdk/util/gdd/descriptionprotocolbase.h 
    ./src/ugdk/util/gdd/loader.h 
    ./src/ugdk/util/gdd/parser.cc 
    ./src/ugdk/util/gdd/parser.h 
    ./src/ugdk/util/gdd/parserutility.h 
    ./src/ugdk/util/gdd/reader.cc 
    ./src/ugdk/util/gdd/reader.h 
    ./src/ugdk/util/gdd/typeconverter.h 
    ./src/ugdk/util/gdd/typedargsconverter.h 
    ./src/ugdk/util/gdd/types.h 
    ./src/ugdk/util/idgenerator.cc 
    ./src/ugdk/util/idgenerator.h 
    ./src/ugdk/util/indexabletable.h 
    ./src/ugdk/util/intervalkdtree.h 
    ./src/ugdk/util/language.cc 
    ./src/ugdk/util/language.h 
    ./src/ugdk/util/languagemanager.cc 
    ./src/ugdk/util/languagemanager.h 
    ./src/ugdk/util/languageword.cc 
    ./src/ugdk/util/languageword.h 
    ./src/ugdk/util/pathmanager.cc 
    ./src/ugdk/util/pathmanager.h 
    ./src/ugdk/util/uncopyable.h 
    ./src/ugdk/util/utf8.cc 
    ./src/ugdk/util/utf8.h 
)
