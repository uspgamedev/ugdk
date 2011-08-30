#ifndef HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
#define HORUSEYE_FRAMEWORK_TIMEHANDLER_H_

#include <ugdk/base/types.h>

namespace ugdk {

// TODO(HenriqueG): descricao de como a classe TimeHandler foi(sera) implementada
class TimeHandler {
  public:
    // Construtores e destrutores
    TimeHandler();
    ~TimeHandler() { }

    // Informa ao TimeHandler que uma nova iteracao iniciou
    void Update();

    // Devolve quanto tempo se passou desde a inicializacao, em milisegundos
    uint32 TimeElapsed();

    // Devolve quanto tempo se passou desde a iteracao anterior, em milisegundos
    uint32 TimeDifference();

    // Devolve quanto tempo se passou desde um ponto no tempo
    uint32 TimeSince(uint32 t0);

    void Pause();
    void Resume();
    bool IsPaused();

  private:
    uint32 initial_time_, current_time_, last_update_, time_paused_;
    bool paused_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
