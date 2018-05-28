#ifndef EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDSIZEVECTOR
#define EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDSIZEVECTOR 1
#pragma once

#include <map>
#include <string.h>

template <typename CLASS_TYPE>
class FixedSizeVector
{
public:

  FixedSizeVector( uint32 maxElements, bool autoRelease ) : 
    m_Max( maxElements ), 
    m_Num( 0 ),
    m_AutoRelease( autoRelease )
  {
    m_Map = new CLASS_TYPE [ m_Max ];
    memset( m_Map, 0, sizeof( CLASS_TYPE ) * m_Max );
  }

  ~FixedSizeVector()
  {
    if ( m_AutoRelease )
    {
      for ( uint32 index = 0; index < m_Num; index++ )
      {
        CLASS_TYPE obj = m_Map[ index ];
        delete obj;
      }
    }
    delete m_Map;
  }

  CLASS_TYPE Get( uint32 index ) const
  {
    if ( index < m_Max )
    {
      CLASS_TYPE obj = m_Map[ index ];
      return obj;
    }
    dfAssertFMT( false, "FixedMultimap::Get. Index [%d] outside of map[%d]", index, m_Max );
    return NULL;
  }

  uint32 Add( CLASS_TYPE obj )
  {
    if ( m_Num < m_Max )
    {
#ifdef DEBUG
      CLASS_TYPE foundObj = m_Map[ m_Num ];
      dfAssert( foundObj == NULL, "FixedMultimap::Add. Map already has an object!" );
#endif
      m_Map[ m_Num ] = obj;
      return m_Num++;
    }
    dfAssertFMT( false, "FixedSizeVector::Add. Index [%d] outside of map[%d]", m_Num, m_Max );
    return m_Max;
  }

  CLASS_TYPE Insert( uint32 index, CLASS_TYPE obj )
  {
    if ( index < m_Max )
    {
      m_Map[ m_Num++ ]  = m_Map[ index ];
      m_Map[ index ]    = obj;
      return obj;
    }
    dfAssertFMT( false, "FixedSizeVector::Add. Index [%d] outside of map[%d]", m_Num, m_Max );
    return NULL;
  }

  bool Remove( CLASS_TYPE removeObj )
  {
    for ( uint32 index = 0; index < m_Num; index++ )
    {
      CLASS_TYPE obj = m_Map[ index ];
      if ( obj == removeObj )
      {
        if ( index < m_Num - 1 )
        {
          m_Map[ index ] = m_Map[ m_Num - 1 ];
        }
        m_Map[ m_Num - 1 ] = NULL;

        if ( m_AutoRelease )
        {
          delete removeObj;
        }
        m_Num--;
        return true;
      }
    }
    return false;
  }

  bool Remove( uint32 index, CLASS_TYPE obj )
  {
    if ( index < m_Num && m_Num > 0 )
    {
      CLASS_TYPE foundObj = m_Map[ index ];
      if ( foundObj == obj )
      {
        if ( index < m_Num - 1 )
        {
          m_Map[ index ] = m_Map[ m_Num - 1 ];
        }
        m_Map[ m_Num - 1 ] = NULL;

        if ( m_AutoRelease )
        {
          delete foundObj;
        }
        m_Num--;
        return true;
      }
    }
    else
    {
      dfAssertFMT( false, "FixedSizeVector::Remove. Index [%d] outside of map[%d]", index, m_Max );
    }
    return false;
  }

  void Clear()
  {
    for ( uint32 index = 0; index < m_Num; index++ )
    {
      CLASS_TYPE obj = m_Map[ index ];
      m_Map[ index ] = NULL;
      if ( m_AutoRelease )
      {
        delete obj;
      }
    }
    m_Num = 0;
  }

  uint32                      GetSize() const{ return m_Num; }
  uint32                      GetMaxSize() const{ return m_Max; }

private:

  uint32                      m_Max;
  uint32                      m_Num;
  bool                        m_AutoRelease;
  CLASS_TYPE *                m_Map;
};

#endif /// EVILENGINE_CORELIB_BASICTYPES_FIXEDMAPS_FIXEDSIZEVECTOR
