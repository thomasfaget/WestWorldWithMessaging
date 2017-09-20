#include "Boozer.h"
#include <random>

bool Boozer::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Boozer::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);


	m_pStateMachine->Update();
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
