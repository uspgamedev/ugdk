#ifndef HORUSEYE_FRAMEWORK_SCENE_H_
#define HORUSEYE_FRAMEWORK_SCENE_H_

#include "layer.h"
#include <list>

namespace ugdk {

// Classe Scene
// Representa uma CENA, que contem varias camadas de graficos.
// Uma cena e' uma "tela" inteira do jogo, como por exemplo o main menu,
// ou o jogo em si (onde ocorre a acao).
class Scene {
  public:
    // The Evul Constructor!
    Scene() : finished_(false), visible_(true) {}
    // The Good Destructor!
    virtual ~Scene();

    // Funcoes basicas pra adicionar e remover layers.
    void AddLayer(Layer *layer) { layers_.push_back(layer); };
    void RemoveLayer(Layer *layer) { layers_.remove(layer); };

    // Termina a cena.
    void Finish() { End(); finished_ = true; }

    // Atualiza a logica da cena.
    virtual void Update(float delta_t);

    // Redesenha o grafico da cena.
    void Render();

    // Redesenha as fontes de luz.
    void RenderLight();

    // Getters.
    bool finished() { return finished_; }

    bool visible () { return visible_; }
    void set_visible (bool set) { visible_ = set; }

  protected:

    // Encerra a atividade da cena.
    virtual void End() {}

    // Lista das camadas.
    ::std::list<Layer*> layers_;
    // Bool que diz se a cena esta' terminada ou nao.
    bool finished_;
    bool visible_;

}; // class Scene.

}

#endif /* HORUSEYE_FRAMEWORK_SCENE_H_ */
