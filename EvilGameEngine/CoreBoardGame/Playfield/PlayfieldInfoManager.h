#ifndef EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFOMANAGER
#define EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFOMANAGER 1
#pragma once

#include "PlayfieldInfo.h"

class PlayfieldInfoManager
{
public:
  PlayfieldInfoManager();
  ~PlayfieldInfoManager();

  static PlayfieldInfoManager* GetInstance();

  void                          AddPlayfield( PlayfieldInfo* playfield );
  PlayfieldInfo*                GetPlayfield( uint32 playfieldId );
  uint32                        GetRandomPlayfield();

  virtual uint32                GetDoors( uint32 /*resourceId*/ ){ return 0;}

protected:

  static PlayfieldInfoManager*  s_Instance;

  PlayfieldInfoMap              m_Playfields;
  std::vector< uint32 >         m_PlayfieldIds;
};

#endif /// EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFOMANAGER