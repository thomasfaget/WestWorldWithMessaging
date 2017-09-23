#include "AIThread.h"
#include <thread>

// This class is a thread which contains a BaseGameEntity.
// This BaseGameEntity can be update (inkoving Update()) on a new thread
AIThread::AIThread(BaseGameEntity* entity) {
    AIThread::entity = entity;
    AIThread::thread = std::thread(std::bind(&AIThread::threadTask, this)); // Associate the thread with the function
}

AIThread::~AIThread() {
    thread.detach();
}


// Set the entity to update
void AIThread::setGameEntity(BaseGameEntity* entity) {
    AIThread::entity = entity;
}

// Update the entity on the thread
void AIThread::update() {
    thread.join();
}

// private task of the thread
void AIThread::threadTask() const {
    entity->Update();
}

