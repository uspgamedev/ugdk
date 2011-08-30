#ifndef HORUSEYE_FRAMEWORK_LAYER_H_
#define HORUSEYE_FRAMEWORK_LAYER_H_

#include <vector>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/sprite.h>

namespace ugdk {

// Representa uma camada grafica na qual os Sprites sao desenhados.
//
// Um objeto Layer pode ter um offset, que e' o deslocamento da camada inteira
// com relacao 'a origem da tela(fullscreen) ou janela.
//
class Layer {
  public:
    // Construtores e destrutores.
    Layer(Vector2D offset = Vector2D()) : visible_(true), light_type_(LIGHT_IGNORE) {}
    virtual ~Layer();

    // Acesso e leitura de atributos.
    bool IsVisible() { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }
    void set_offset(Vector2D offset) { offset_ = offset; }
    Vector2D offset() { return offset_; }
    void set_light_type(LightType type) { light_type_ = type; }
    LightType light_type() { return light_type_; }

    // Adicionando e removendo Sprites.
    void AddSprite(Sprite *sprite);
    void RemoveSprite(Sprite *sprite);

    // Atualiza a camada e seus Sprites.
    virtual void Update(float delta_t);

    // Desenha a camada.
    virtual void Render();
	virtual void RenderLight();

  protected:
    // Estrutura basica para armazenar os Sprites. Nao definitivo.
    std::vector<Sprite*> sprite_list_;
    bool visible_;

  private:
    // Atributos.
    Vector2D offset_;

    LightType light_type_;

    // Ordena os sprites pelo zindex
    virtual void SortSprites();

}; // class Layer.

}

#endif // HORUSEYE_FRAMEWORK_LAYER_H_
