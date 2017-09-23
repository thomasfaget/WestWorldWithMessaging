#include "AIThreadManager.h"

AIThreadManager::AIThreadManager() {}

// Add a thread in the manager
void AIThreadManager::addThread(AIThread* thread) {
    threadList.push_back(thread);
}

// Update all the thread
void AIThreadManager::updateAllThreads() {
    for (int i = 0; i < threadList.size(); ++i) {
        threadList.at( i )->update();
    }
}

// delete all the thread
void AIThreadManager::clear() {
    for (int i = 0; i < threadList.size(); ++i) {
        delete threadList.at( i );
    }
}
