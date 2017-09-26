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
 
  m_pStateMachine->Update();
  this->unlock();
}

void MinersWife::speak(std::string msg, MinersWife* wife) {
	ConsoleUtils::getInstance().PrintMessageInConsole("\n" + GetNameOfEntity(wife->ID()) + " : " + msg, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}