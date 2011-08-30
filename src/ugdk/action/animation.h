#ifndef HORUSEYE_FRAMEWORK_ANIMATION_H_
#define HORUSEYE_FRAMEWORK_ANIMATION_H_

#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class Observer;
class AnimationSet;

/*
 * Represents a sprite's current animation.
 *
 * An Animation object contains a sequence of information describing a sprite's visual behavior
 * (spritesheet frame, position, rotation, transparency, etc) throughout a corresponding sequence
 * of the game's update frames. This per-frame information is represented by the class
 * Animation::AnimationFrame, and the sequence of this information by the Animation::FrameSequence
 * type.
 *
 * Also, an Animation object contains a set of all the possible animations the sprite may need.
 * This set is represented by the AnimationSet class. Thus, a sprite needs but one Animation object
 * to access any of the animations it requires, as long as these are all properly registered
 * in the AnimationSet object given to the Animation object.
 */
class Animation {

  public:

    /*
     * Represents the visual behavior information of a sprite in a single game frame.
     */
    class AnimationFrame {

      public:

        /*
         * frame: the index of the spritesheet frame that should be rendered.
         * modifier: a pointer to the Modifier object describing the visual modifiers that
         *  should be applied to the rendered sprite.
         */
        AnimationFrame(int frame, Modifier *modifier = NULL)
            : frame_(frame), modifier_(modifier) {}

        int frame() const { return frame_; }
        Modifier *modifier() const { return modifier_; }
      private:
        int frame_;
        Modifier *modifier_;
    };

    typedef std::vector<AnimationFrame*> FrameSequence;

	// Primeiro parametro define o fps da animacao.
	// Os parametros seguintes sao os indices dos frames da animacao.
	// Lista de parametro e' encerrada com um -1
	//
	//  A imagem e' dividida usando o frame_size, e o indice de um frame
	//  e' representado pela sua posicao na imagem fonte, em linhas, da
	//  esquerda para a direita e de cima para baixo.
    //Animation(float fps, ...);

    Animation(float fps, AnimationSet *set);
    ~Animation();

    //void set_framelist(int frame1, ...);
    void set_fps(float fps) { period_ = 1.0f/fps; }
    void set_period(float period) { period_ = period; }
    float fps() { return 1.0/period_; }
    float period() { return period_; }
    int n_frames() { return frames_->size(); }

    int GetFrame();
    void set_default_frame(int default_frame) {
        default_frame_ = default_frame;
    }
    const Modifier* get_current_modifier() const {
        return frames_
                ? frames_->at(current_frame_)->modifier()
                : NULL;
    }
    void Select(std::string name);
    void Select(int index);
    void Update(float delta_t);
    void AddObserver(Observer* observer);

  private:
    float period_;
    FrameSequence *frames_;
    AnimationSet *animation_set_;
    //int n_frames_;
    int current_frame_;
    int default_frame_;
    float elapsed_time_;
    std::vector<Observer *> observers;
    void NotifyAllObservers();

    //void CopyFrameList(int frame_list[], int n_frames);

};

}
#endif


