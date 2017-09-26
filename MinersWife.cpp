#include "MinersWife.h"
#include "EntityNames.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update()
{
	this->lock();
  //set text color to green
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  m_pStateMachine->Update();
  this->unlock();
}

void MinersWife::speak(std::string msg) {
	ConsoleUtils::getInstance().PrintMessageInConsole("\n" + GetNameOfEntity(this->ID()) + " : " + msg, FOREGROUND_GREEN | FOREGROUND_INTENSITY); 
}