#include "BaseGameEntity.h"
#include <cassert>
#include <chrono>
#include <thread>

int BaseGameEntity::m_iNextValidID = 0;



//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void BaseGameEntity::SetID(int val)
{
  //make sure the val is equal to or greater than the next available ID
  assert ( (val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");

  m_ID = val;
    
  m_iNextValidID = m_ID + 1;
}

// called when thread starts to run
void BaseGameEntity::run() {
	for (int i = 0; i < 30; i++) {
		this->Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}
}