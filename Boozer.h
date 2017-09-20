#ifndef BOOZER_H
#define BOOZER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "BoozerOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

// The life max of the boozer
const int lifeMax2 = 5;
// number of turns until the KO pnj wake up:
const int koDelay2 = 5;

class Boozer : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Boozer>*  m_pStateMachine;

	location_type         m_Location;

	int life; // His life
	int koLevel; // The current number of turns  being KO


public:

	Boozer(int id) :m_Location(saloon),
		BaseGameEntity(id),
		life(lifeMax2),
		koLevel(0)

	{
		//set up state machine
		m_pStateMachine = new StateMachine<Boozer>(this);

		m_pStateMachine->SetCurrentState(DrinkB::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	}

	~Boozer() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

	// Handle life :
	void DecreaseLife() { life--; }
	void ResetLife() { life = lifeMax2; }
	bool IsKO() const { return life == 0; }

	// Handle KO :
	void IncreaseKoLevel() { koLevel++; }
	void ResetKoLevel() { koLevel = 0; }
	bool IsStunned() const { return koLevel == koDelay2;  }


	StateMachine<Boozer>* GetFSM()const { return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }

	// if true attack succes else no 
	bool          TryToPunch();

};



#endif

