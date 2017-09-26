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


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  Boozer* Booz = new Boozer(ent_Boozer);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(Booz);

  //run Bob and Elsa through a few Update calls

  Bob->start();
  Elsa->start();
  Booz->start();
  Dispatch->start();

  // wait all the thread to finish their work
  Bob->join();
  Elsa->join();
  Booz->join();
  Dispatch->join();

  //tidy up
  delete Bob;
  delete Elsa;
  delete Booz;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






