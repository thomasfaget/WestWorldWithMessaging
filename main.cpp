#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Boozer.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#include "AIThread.h"


std::ofstream os;

int main() {
    //define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

    //seed random number generator
    srand((unsigned)time(NULL));

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

    //run Bob and Elsa through a few Update calls
    for (int i = 0; i < 30; ++i) {
        
        minerThread.update();
        minerWifeThread.update();
        boozerThread.update();

        //dispatch any delayed messages
        Dispatch->DispatchDelayedMessages();

        Sleep(800);
    }

    //tidy up
    delete bob;
    delete elsa;
    delete boozer;

    //wait for a keypress before exiting
    PressAnyKeyToContinue();

    return 0;
}
