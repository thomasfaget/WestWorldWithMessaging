#include "Boozer.h"
#include "EntityNames.h"
#include <random>

extern std::mutex lock_speak;

bool Boozer::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Boozer::Update()
{
	this->lock();


	m_pStateMachine->Update();

	this->unlock();
}

bool Boozer::TryToPunch()
{
	int rand = std::rand() % 2;
	if (rand == 0) {
		// Success his punch :
		return true;
	}
	else {
		// Fail to punch :
		return false;
	}
}

void Boozer::speak(std::string msg, Boozer* pBoozer) {
	ConsoleUtils::getInstance().PrintMessageInConsole("\n" + GetNameOfEntity(pBoozer->ID()) + " : " + msg, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
