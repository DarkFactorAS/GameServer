
#include "PreCompile.h"
#include "PlayfieldSource.h"

PlayfieldSource::PlayfieldSource() :
  m_SourceType(0),
  m_SourceId(0)
{
}

PlayfieldSource::PlayfieldSource(uint32 sourceType, uint32 sourceId, String tokenId) :
  m_SourceType(sourceType),
  m_SourceId(sourceId),
  m_TokenID(tokenId)
{

}
