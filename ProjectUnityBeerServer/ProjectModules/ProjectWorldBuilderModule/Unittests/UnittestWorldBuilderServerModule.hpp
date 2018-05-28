#pragma once

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

class UnittestWorldBuilderServerModule : public ProjectWorldBuilderServerModule
{
private:
  std::vector< Playfield* > m_PlayfieldList;

public:
  void AddPlayfield(Playfield* playfield)
  {
    m_PlayfieldList.push_back(playfield);
  }

  virtual Playfield* LoadPlayfield(uint32 /*accountId*/, uint32 playfieldId) DF_OVERRIDE
  {
    uint32 numPlayfields = m_PlayfieldList.size();
    for (uint32 index = 0; index < numPlayfields; index++)
    {
      Playfield* playfield = m_PlayfieldList[index];
      if (playfield != NULL && playfield->GetPlayfieldId() == playfieldId)
      {
        return playfield;
      }
    }
    return NULL;
  }
};
