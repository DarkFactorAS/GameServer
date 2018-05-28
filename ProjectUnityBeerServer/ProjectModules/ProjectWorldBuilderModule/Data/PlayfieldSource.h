#ifndef PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDSOURCE
#define PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDSOURCE 1

class PlayfieldSource
{
public:

  PlayfieldSource();
  PlayfieldSource(uint32 sourceType, uint32 sourceId, String tokenId);

  uint32 m_SourceType;
  uint32 m_SourceId;
  String m_TokenID;
};

#endif /// PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDSOURCE
