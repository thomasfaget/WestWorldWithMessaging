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
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

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

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

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
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
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
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Depositing gold. Total savings now: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      
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
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
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
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

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
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
          << "All mah fatigue has drained away. Time to find more gold!";

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID()) 
          << ": Okay Hun, ahm a comin'!";

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

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";

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
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "I drink too much ";
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{    
	switch (msg.Msg) {

		case Msg_ImDrinking: 

			cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) << " at time: " << Clock->GetCurrentTime();

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
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
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


void Fight::Enter(Miner* pBoozer) {

	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " : I'm gonna kick your ass motherfucker !";
	

}


void Fight::Execute(Miner* pBoozer) {

	// He tries to punch boozer
	bool punch = pBoozer->TryToPunch();
	if (punch) {
		// He sends him a msg if he successes his punch
		Dispatch->DispatchMessage(
			SEND_MSG_IMMEDIATELY,
			pBoozer->ID(),
			ent_Boozer,
			Msg_IPunchYou,
			NO_ADDITIONAL_INFO);
	}
}

void Fight::Exit(Miner* pBoozer) {
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " : Lets leave this fucking saloon ! ";
}

bool Fight::OnMessage(Miner* pBoozer, const Telegram& msg) {

	switch (msg.Msg) {

		case Msg_IPunchYou: 

			// The Miner recieves a punch :
			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			pBoozer->DecreaseLife();
			if (pBoozer->IsKO()) {
				// If KO : Send a message to the boozer :
				Dispatch->DispatchMessage(
					SEND_MSG_IMMEDIATELY,
					pBoozer->ID(),
					ent_Miner_Bob,
					Msg_ImKO,
					NO_ADDITIONAL_INFO);

				// Change state to KO :
				pBoozer->GetFSM()->ChangeState(KO::Instance());
			}
			return true;

		case Msg_ImKO:
		
			cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID()) << " at time: " << Clock->GetCurrentTime();

			// If the boozer is KO, he go back in the mine :
			pBoozer->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
			pBoozer->ResetLife();
			return true;
		

	}
	return false;

}

KO* KO::Instance()
{
	static KO instance;

	return &instance;
}


void KO::Enter(Miner* pBoozer) {
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " : Is KO !";
}


void KO::Execute(Miner* pBoozer) {
	// increase Ko level
	pBoozer->IncreaseKoLevel();
	bool change = pBoozer->IsStunned();
	if (change) {
		pBoozer->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}


}


void KO::Exit(Miner* pBoozer) {
	//Exit of Ko state reset it 
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " : What happened ? Well ... Let's moove !";
	pBoozer->ResetKoLevel();
}


bool KO::OnMessage(Miner* pBoozer, const Telegram& msg) {
	return false;
}


