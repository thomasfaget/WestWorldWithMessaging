#include <fstream>
#include <time.h>
#include <mutex>

#include "Miner.h"
#include "MinersWife.h"
#include "Boozer.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

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

	//register them with the entity manager
	EntityMgr->RegisterEntity(bob);
	EntityMgr->RegisterEntity(elsa);
	EntityMgr->RegisterEntity(boozer);


	//run Bob and Elsa through a few Update calls

	bob->start();
	elsa->start();
	boozer->start();
	Dispatch->start();

	// wait all the thread to finish their work
	bob->join();
	elsa->join();
	boozer->join();
	Dispatch->join();

	//tidy up
	delete bob;
	delete elsa;
	delete boozer;

	//wait for a keypress before exiting
	PressAnyKeyToContinue();

	return 0;
}
