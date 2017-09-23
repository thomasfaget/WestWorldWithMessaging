#include <vector>
#include "AIThread.h"

#ifndef AITHREADMANAGER_H
#define AITHREADMANAGER_H


class AIThreadManager {

private:

    std::vector<AIThread*> threadList;

public:

    AIThreadManager();
    void addThread( AIThread* thread );
    void updateAllThreads();
    void clear();
    
};
#endif // AITHREADMANAGER_H
