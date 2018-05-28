
#include "Precompile.h"
#include "PlayfieldInfoManager.h"

#include <stdlib.h>

PlayfieldInfoManager* PlayfieldInfoManager::s_Instance = NULL;

PlayfieldInfoManager::PlayfieldInfoManager()
{
}

PlayfieldInfoManager::~PlayfieldInfoManager()
{
  for ( PlayfieldInfoMap::iterator itPlayfield = m_Playfields.begin(); itPlayfield != m_Playfields.begin(); ++itPlayfield )
  {
    PlayfieldInfo* playfieldInfo = itPlayfield->second;
    delete playfieldInfo;
  }
}

PlayfieldInfoManager* PlayfieldInfoManager::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new PlayfieldInfoManager();
  }
  return s_Instance;
}

void PlayfieldInfoManager::AddPlayfield( PlayfieldInfo* playfield )
{
  m_Playfields[ playfield->GetPlayfieldId() ] = playfield;
  m_PlayfieldIds.push_back( playfield->GetPlayfieldId() );
}

uint32 PlayfieldInfoManager::GetRandomPlayfield()
{
  if ( !m_PlayfieldIds.empty() )
  {
    uint32 pIndex       = rand() % m_PlayfieldIds.size();
    uint32 playfieldId  = m_PlayfieldIds[ pIndex ];
    PlayfieldInfo* info = GetPlayfield( playfieldId );
    if ( info != NULL )
    {
      return info->GetPlayfieldId();
    }
  }
  return 0;
}
PlayfieldInfo* PlayfieldInfoManager::GetPlayfield( uint32 playfieldId )
{
  PlayfieldInfoMap::iterator itPf = m_Playfields.find( playfieldId );
  if ( itPf != m_Playfields.end() )
  {
    return itPf->second;
  }
  return NULL;
}

