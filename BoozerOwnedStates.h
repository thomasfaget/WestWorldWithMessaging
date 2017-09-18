#ifndef BOOZER_OWNED_STATES_H
#define BOOZER_OWNED_STATES_H

#include "fsm/State.h"


class Boozer;
struct Telegram;


class DrinkB : public State<Boozer>
{
private:

	DrinkB() {}

	//copy ctor and assignment should be private
	DrinkB(const DrinkB&);
	DrinkB& operator=(const DrinkB&);

public:

	//this is a singleton
	static DrinkB* Instance();

	virtual void Enter(Boozer* boozer){}

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer){}

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);

};

class FightB : public State<Boozer>
{
private:

	FightB() {}

	//copy ctor and assignment should be private
	FightB(const FightB&);
	FightB& operator=(const FightB&);

public:

	//this is a singleton
	static FightB* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);

};

class KOB : public State<Boozer>
{
private:

	KOB() {}

	//copy ctor and assignment should be private
	KOB(const KOB&);
	KOB& operator=(const KOB&);

public:

	//this is a singleton
	static KOB* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);

};



#endif