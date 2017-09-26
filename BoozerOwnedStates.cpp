#include "BoozerOwnedStates.h"
#include "fsm/State.h"
#include "Boozer.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


// ------------- Drink State -----------------
DrinkB* DrinkB::Instance()
{
	static DrinkB instance;

	return &instance;
}


void DrinkB::Execute(Boozer* pBoozer) {

	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " : I'm Drinking bitch !!!";

}

bool DrinkB::OnMessage(Boozer* pBoozer, const Telegram& msg) {

	

	switch (msg.Msg) {

		case Msg_ImDrinking: 

			SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			// tell Bob to enter the fight immediately
			Dispatch->DispatchMessage(
				SEND_MSG_IMMEDIATELY,
				pBoozer->ID(),
				ent_Miner_Bob,
				Msg_WannaFight,
				NO_ADDITIONAL_INFO);

			return true;

		case Msg_AcceptFight:

			SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			pBoozer->GetFSM()->ChangeState(FightB::Instance());

			return true;
		

	}
	return false;
}

// ------------- Fight State -----------------
FightB* FightB::Instance()
{
	static FightB instance;

	return &instance;
}


void FightB::Enter(Boozer* pBoozer) {
	pBoozer->speak("Hey look at that stupid face, haha!");
}


void FightB::Execute(Boozer* pBoozer) {

	// He tries to punch Bob

	pBoozer->speak("Fighting...");

	
	bool punch = pBoozer->TryToPunch();
	if (punch) {
		pBoozer->speak("Take this Miner ! ");
		// He sends him a msg if he successes his punch
		Dispatch->DispatchMessage(
			SEND_MSG_IMMEDIATELY,
			pBoozer->ID(),
			ent_Miner_Bob,
			Msg_IPunchYou,
			NO_ADDITIONAL_INFO);
	}

}

void FightB::Exit(Boozer* pBoozer) {

	// He resets his lifewhen he is KO
	pBoozer->ResetLife();
}

bool FightB::OnMessage(Boozer* pBoozer, const Telegram& msg) {

	switch (msg.Msg) {

		case Msg_IPunchYou: 

			SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
			// The boozer recieves a punch :
			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			pBoozer->DecreaseLife();

			if (pBoozer->IsKO()) {
				// If KO : Send a message to the miner :
				Dispatch->DispatchMessage(
					SEND_MSG_IMMEDIATELY,
					pBoozer->ID(),
					ent_Miner_Bob,
					Msg_ImKO,
					NO_ADDITIONAL_INFO);

				// Change state to KO :
				pBoozer->GetFSM()->ChangeState(KOB::Instance());
			}
			return true;
		

		case Msg_ImKO:

			SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			pBoozer->speak("What a stupid fight...");

			// If the miner is KO, he go back DrinkB and reset his life :
			pBoozer->GetFSM()->ChangeState(DrinkB::Instance());
			return true;
		

	}
	return false;

}

// ------------- KO State -----------------
KOB* KOB::Instance()
{
	static KOB instance;

	return &instance;
}


void KOB::Enter(Boozer* pBoozer) {
}


void KOB::Execute(Boozer* pBoozer) {

	pBoozer->speak("I'm KO !");

	// increase Ko level
	pBoozer->IncreaseKoLevel();
	bool change = pBoozer->IsStunned();
	if (change) {
		pBoozer->GetFSM()->ChangeState(DrinkB::Instance());
	}
				

}


void KOB::Exit(Boozer* pBoozer) {
	//Exit of Ko state reset it 
	pBoozer->speak("What happened ? Well ... Let's drink again !");
	pBoozer->ResetKoLevel();
}


bool KOB::OnMessage(Boozer* pBoozer, const Telegram& msg) {
	return false;
}

