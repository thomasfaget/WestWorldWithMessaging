#include "Miner.h"
#include <random>

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void Miner::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  if ((!isFighting) && (koLevel == 0)) { // si il n'est ni en combat ni en état de ko 
	  m_iThirst += 1;
  }
  
  m_pStateMachine->Update();
}



void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}

bool Miner::TryToPunch()
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
