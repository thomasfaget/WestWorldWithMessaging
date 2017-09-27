#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
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


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		pMiner->speak("Walkin' to the goldmine");

		pMiner->ChangeLocation(goldmine);
	}
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	pMiner->speak("Pickin' up a nugget");

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	pMiner->speak("Ah'm leavin' the goldmine with mah pockets full o' sweet gold");
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		pMiner->speak("Goin' to the bank. Yes siree");

		pMiner->ChangeLocation(bank);
	}
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);

	pMiner->speak("Depositing gold. Total savings now: " + std::to_string(pMiner->Wealth()));

	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{

		pMiner->speak("WooHoo!Rich enough for now.Back home to mah li'lle lady");

		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	pMiner->speak("Leavin' the bank");
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		pMiner->speak("Walkin' home");

		pMiner->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		pMiner->speak("All mah fatigue has drained away. Time to find more gold!");

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		pMiner->speak("ZZZZ... ");
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{

	switch (msg.Msg)
	{
	case Msg_StewReady:
	    const std::string message = "Message handled by " + GetNameOfEntity(pMiner->ID())
			+ " at time: " + std::to_string(Clock->GetCurrentTime());

		ConsoleUtils::getInstance().PrintMessageInConsole(message, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


		pMiner->speak("Okay Hun, ahm a comin'!");

		pMiner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);

		pMiner->speak("Boy, ah sure is thusty! Walking to the saloon");

	}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	pMiner->BuyAndDrinkAWhiskey();

	pMiner->speak("That's mighty fine sippin' liquer");

	// tell the boozer Bob is here
	Dispatch->DispatchMessage(
		SEND_MSG_IMMEDIATELY,
		pMiner->ID(),
		ent_Boozer,
		Msg_ImDrinking,
		NO_ADDITIONAL_INFO);
}


void QuenchThirst::Exit(Miner* pMiner)
{
	pMiner->speak("I drink too much ");
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{

	switch (msg.Msg) {

	case Msg_WannaFight:

	    const std::string message = "Message handled by " + GetNameOfEntity( pMiner->ID() )
            + " at time: " + std::to_string( Clock->GetCurrentTime() );

        ConsoleUtils::getInstance().PrintMessageInConsole( message, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );


		// ack
		Dispatch->DispatchMessage(
			SEND_MSG_IMMEDIATELY,
			pMiner->ID(),
			ent_Boozer,
			Msg_AcceptFight,
			NO_ADDITIONAL_INFO);

		pMiner->GetFSM()->ChangeState(Fight::Instance());

		return true;

	}

	return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	pMiner->speak("Smells Reaaal goood Elsa!");
}

void EatStew::Execute(Miner* pMiner)
{
	pMiner->speak("Tastes real good too!");

	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	pMiner->speak("Thankya li'lle lady. Ah better get back to whatever ah wuz doin'");
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------Fight
Fight* Fight::Instance()
{
	static Fight instance;

	return &instance;
}


void Fight::Enter(Miner* pMiner) {
	pMiner->StartFight();
	pMiner->speak("You started this fight !");
}


void Fight::Execute(Miner* pMiner) {

	pMiner->speak("Fighting...");

	// He tries to punch boozer
	bool punch = pMiner->TryToPunch();
	if (punch) {
		pMiner->speak("Take this Boozer !");
		// He sends him a msg if he successes his punch
		Dispatch->DispatchMessage(
			SEND_MSG_IMMEDIATELY,
			pMiner->ID(),
			ent_Boozer,
			Msg_IPunchYou,
			NO_ADDITIONAL_INFO);
	}

}

void Fight::Exit(Miner* pMiner) {
	pMiner->StopFight();
}

bool Fight::OnMessage(Miner* pMiner, const Telegram& msg) {



	switch (msg.Msg) {

	case Msg_IPunchYou:
	{

	    const std::string message = "Message handled by " + GetNameOfEntity(pMiner->ID())
			+ " at time: " + std::to_string(Clock->GetCurrentTime());

		ConsoleUtils::getInstance().PrintMessageInConsole(message, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		pMiner->DecreaseLife();

		if (pMiner->IsKO()) {
			// If KO : Send a message to the boozer :
			Dispatch->DispatchMessage(
				SEND_MSG_IMMEDIATELY,
				pMiner->ID(),
				ent_Boozer,
				Msg_ImKO,
				NO_ADDITIONAL_INFO);

			// Change state to KO :
			pMiner->GetFSM()->ChangeState(KO::Instance());
		}
		return true;
	}

	case Msg_ImKO:
	{

	    const std::string message = "Message handled by " + GetNameOfEntity(pMiner->ID())
			+ " at time: " + std::to_string(Clock->GetCurrentTime());

		ConsoleUtils::getInstance().PrintMessageInConsole(message, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		pMiner->speak("Lets leave this fucking saloon ! ");

		// If the boozer is KO, he go back in the mine :
		pMiner->ResetLife();
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());

		return true;


	}
	return false;
	}

}

KO* KO::Instance()
{
	static KO instance;

	return &instance;
}


void KO::Enter(Miner* pMiner) {
}


void KO::Execute(Miner* pMiner) {

	pMiner->speak("I'm KO !");

	// increase Ko level
	pMiner->IncreaseKoLevel();
	bool change = pMiner->IsStunned();
	if (change) {
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}


}


void KO::Exit(Miner* pMiner) {
	//Exit of Ko state reset it 
	pMiner->speak("What happened ? Well ... Let's moove !");

	pMiner->ResetKoLevel();
}


bool KO::OnMessage(Miner* pMiner, const Telegram& msg) {
	return false;
}


