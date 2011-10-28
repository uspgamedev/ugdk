#ifndef HORUSEYE_FRAMEWORK_SCENE_H_
#define HORUSEYE_FRAMEWORK_SCENE_H_

#include <ugdk/action/layer.h>
#include <list>

namespace ugdk {

// Classe Scene
// Representa uma CENA, que contem varias camadas de graficos.
// Uma cena e' uma "tela" inteira do jogo, como por exemplo o main menu,
// ou o jogo em si (onde ocorre a acao).

/**
   @class Scene
   @brief A game scene, or screen.

   Represents a scene, containing various graphical layers and sprites.
   A scene is game screen, such as a menu, or the playable part
   itself.
*/
class Scene {
  public:
    Scene() : finished_(false), visible_(true) {}
  /**
     Note: Destroys all layers in the scene.
  */
    virtual ~Scene();

    // Funcoes basicas pra adicionar e remover layers.
    /// Adds a Layer to the scene on top of current layers.
    void AddLayer(Layer *layer) { layers_.push_back(layer); };
    /// Removes the specified Layer from the scene.
    void RemoveLayer(Layer *layer) { layers_.remove(layer); };

    // Termina a cena.
    /// Finishes the scene.
    void Finish() { End(); finished_ = true; }

    // Atualiza a logica da cena.
    /// Logical update of the scene.
    /**
       @param delta_t Time in seconds since last update
    */
    virtual void Update(float delta_t);

    // Redesenha o grafico da cena.
    /// Redraws the scene's graphics.
    void Render();

    // Redesenha as fontes de luz.
    /// Redraws lightsources.
    void RenderLight();

    // Getters.
    /** @name Getters and Setters
	@{
    */

    bool finished() { return finished_; }

    bool visible () { return visible_; }
    void set_visible (bool set) { visible_ = set; }
    /**@}
     */
  protected:

    // Encerra a atividade da cena.
    /// Ends the scene activity.
    virtual void End() {}

    // Lista das camadas.
    /// Layer list
    ::std::list<Layer*> layers_;
    // Bool que diz se a cena esta' terminada ou nao.
    /// Tells whether scene is finished or not.
    bool finished_;
    /// Tells whether scene is visible or not.
    bool visible_;

}; // class Scene.

}

#endif /* HORUSEYE_FRAMEWORK_SCENE_H_ */
