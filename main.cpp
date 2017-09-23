#include <fstream>
#include <time.h>

#include "Miner.h"
#include "MinersWife.h"
#include "Boozer.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#include "AIThread.h"
#include "AIThreadManager.h"


std::ofstream os;

int main() {
    //define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

    //seed random number generator
    srand((unsigned)time(nullptr));

    //create the AI :
    Miner* bob = new Miner(ent_Miner_Bob);
    MinersWife* elsa = new MinersWife(ent_Elsa);
    Boozer* boozer = new Boozer(ent_Boozer);

    // Create the threads :
    AIThread minerThread( bob );
    AIThread minerWifeThread( elsa );
    AIThread boozerThread( boozer );

    //register them with the entity manager
    EntityMgr->RegisterEntity(bob);
    EntityMgr->RegisterEntity(elsa);
    EntityMgr->RegisterEntity(boozer);

    // The ThreadManager and all the thread
    AIThreadManager threadManager;
    threadManager.addThread( &minerThread );
    threadManager.addThread( &minerWifeThread );
    threadManager.addThread( &boozerThread );

    //run Bob and Elsa through a few Update calls
    for (int i = 0; i < 30; ++i) {
        
        // Update all the threads
        threadManager.updateAllThreads();

        //dispatch any delayed messages
        Dispatch->DispatchDelayedMessages();

        Sleep(800);
    }

    //tidy up
    delete bob;
    delete elsa;
    delete boozer;
    threadManager.clear();

    //wait for a keypress before exiting
    PressAnyKeyToContinue();

    return 0;
}
