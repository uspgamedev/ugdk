#ifndef UGDK_TIME_TIMEACCUMULATOR_H_
#define UGDK_TIME_TIMEACCUMULATOR_H_

#include <ugdk/time.h>
#include <ugdk/structure/types.h>

namespace ugdk {
namespace time {

class TimeAccumulator {
  public:
    // Construtores e destrutores
    TimeAccumulator(int duration_miliseconds);
    ~TimeAccumulator() { }

    // Ignora tempo pausado
    int TimeLeft();

    // True se o timer ja expirou
    bool Expired();

    // Despausa se pausado
    void Restart();

    // Despausa se pausado e ataliza a duracao
    void Restart(int duration_miliseconds);

    // Pausa o timer, desconsiderando o tempo ate que seja despausado
    void Pause();

    // Despausa o contador
    void Resume();

    // True se pausado. False caso contrario
    bool IsPaused();

  private:
    Manager &handler_;
    uint32 initial_time_, time_paused_, when_paused_, duration_;
};

}  // namespace time
}  // namespace ugdk

#endif  // HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
