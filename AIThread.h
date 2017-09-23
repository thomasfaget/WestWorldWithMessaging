// This class is a thread which contains a BaseGameEntity.
// This BaseGameEntity can be update (inkoving Update()) on a new thread


#ifndef AITHREAD_H
#define AITHREAD_H
#include "BaseGameEntity.h"
#include <thread>


class AIThread  {

private:

    std::thread thread;
    BaseGameEntity* entity;
    void threadTask() const;

public:

    explicit AIThread( BaseGameEntity* entity );
    ~AIThread();
    void setGameEntity( BaseGameEntity* entity );
    void update();
    
};
#endif // AITHREAD_H
