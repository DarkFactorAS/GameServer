#ifndef EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDMULTIMAP
#define EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDMULTIMAPL 1
#pragma once

#include <map>
#include <string.h>

template <typename CLASS_TYPE>
class FixedMultimap
{
public:

  FixedMultimap( uint32 x, uint32 y, bool autoRelease ) : 
    m_MaxX( x ), 
    m_MaxY( y ), 
    m_AutoRelease( autoRelease )
  {
    m_Max = x * y;
    m_Map = new CLASS_TYPE* [ m_Max ];
    memset( m_Map, 0, sizeof( CLASS_TYPE* ) * m_Max );
  }

  ~FixedMultimap()
  {
    if ( m_AutoRelease )
    {
      for ( uint32 index = 0; index < m_Max; index++ )
      {
        CLASS_TYPE* obj = m_Map[ index ];
        delete obj;
      }
    }
    delete m_Map;
  }

  CLASS_TYPE* Get( uint32 index ) const
  {
    if ( index < m_Max )
    {
      CLASS_TYPE* obj = m_Map[ index ];
      return obj;
    }
    dfAssertFMT( false, "FixedMultimap::Get. Index [%d] outside of map[%d]", index, m_Max );
    return NULL;
  }

  CLASS_TYPE* Get( uint32 indexX, uint32 indexY ) const
  {
    if ( indexX < m_MaxX && indexY < m_MaxY )
    {
      CLASS_TYPE* obj = m_Map[ ( indexY * m_MaxX ) + indexX ];
      return obj;
    }
    dfAssertFMT( false, "FixedMultimap::Get. Index [%d,%d] outside of map[%d,%d]", indexX, indexY, m_MaxX, m_MaxY );
    return NULL;
  }

  CLASS_TYPE* Add( uint32 indexX, uint32 indexY, CLASS_TYPE* obj )
  {
    if ( indexX < m_MaxX && indexY < m_MaxY )
    {
#ifdef DEBUG
      CLASS_TYPE* foundObj = m_Map[ ( indexY * m_MaxX ) + indexX ];
      dfAssert( m_AutoRelease && ( foundObj == NULL ), "FixedMultimap::Add. Map already has an object!" );
#endif
      m_Map[ ( indexY * m_MaxX ) + indexX ] = obj;
      return obj;
    }
    dfAssertFMT( false, "FixedMultimap::Add. Index [%d,%d] outside of map[%d,%d]", indexX, indexY, m_MaxX, m_MaxY );
    return NULL;
  }

  bool Remove( uint32 indexX, uint32 indexY, CLASS_TYPE* obj )
  {
    if ( indexX < m_MaxX && indexY < m_MaxY )
    {
      CLASS_TYPE* foundObj = m_Map[ ( indexY * m_MaxX ) + indexX ];
      if ( foundObj == obj )
      {
        m_Map[ ( indexY * m_MaxX ) + indexX ] = NULL;
        if ( m_AutoRelease )
        {
          delete foundObj;
        }
        return true;
      }
    }
    else
    {
      dfAssertFMT( false, "FixedMultimap::Remove. Index [%d,%d] outside of map[%d,%d]", indexX, indexY, m_MaxX, m_MaxY );
    }
    return false;
  }

  uint32                      GetSize() const{ return m_Max; }

private:

  uint32                      m_Max;
  uint32                      m_MaxX;
  uint32                      m_MaxY;
  bool                        m_AutoRelease;

  CLASS_TYPE **               m_Map;
};

#endif /// EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDMULTIMAP
