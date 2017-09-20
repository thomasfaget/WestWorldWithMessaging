#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_WannaFight,
  Msg_IPunchYou,
  Msg_ImKO
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_WannaFight:

	  return "WannaFight";

  case Msg_IPunchYou:

	  return "IPunchYou";

  case Msg_ImKO:

	  return "ImKO";

  default:

      return "Not recognized!";
  }
}

#endif