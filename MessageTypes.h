#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_ImDrinking,
  Msg_WannaFight,
  Msg_AcceptFight,
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

  case Msg_ImDrinking:

	  return "ImDrinking";

  case Msg_WannaFight:

	  return "WannaFight";

  case Msg_AcceptFight:

	  return "AcceptFight";

  case Msg_IPunchYou:

	  return "IPunchYou";

  case Msg_ImKO:

	  return "ImKO";

  default:

      return "Not recognized!";
  }
}

#endif