/*************************************************************************************
* Project     : EvilEngine CoreLib 2014
* File        : Bundle
* Description : Handles a name, value par list. The value can be any native type
*               ( int, float, String ). This class is used to pass generic data between
*               classes.
*
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "Bundle.h"

/***********************************************************************************
* AddData:
* Add a key, value pair to the bundle list.
* 
* @param (const String&)  name - Name of the key
* @param (Variant)        val  - The value to add to the key
* 
* @author Thor Richard Hansen
***********************************************************************************/
void Bundle::AddData( const String& name, Variant val )
{
  String lowerName = name.GetLower();

  Variant& oldValue = GetInternalData( lowerName );
  if ( oldValue.IsNull() )
  {
    m_Data.push_back( std::pair<String,Variant>( lowerName, val ) );
  }
  else
  {
    oldValue.Copy( val );
  }
}

/***********************************************************************************
* GetData:
* returns a value from the key (if it exist in this Bundle). If the key does not
* exist, a ZERO Variant value is returned.
* 
* @param  (const String&)  name - Name of the key
* @return (Variant)             - The value of the key
* 
* @author Thor Richard Hansen
***********************************************************************************/
const Variant& Bundle::GetData( const String& name ) const
{
  String lowerName = name.GetLower();
  return GetInternalData( lowerName );
}

/***********************************************************************************
* GetInternalData:
* returns a value from the key (if it exist in this Bundle). If the key does not
* exist, a ZERO Variant value is returned. The name is always lower key.
* 
* @param  (const String&)  name - Name of the key
* @return (Variant)             - The value of the key
* 
* @author Thor Richard Hansen
***********************************************************************************/
const Variant& Bundle::GetInternalData( const String& lowerName ) const
{
  for ( size_t index = 0; index < m_Data.size(); index++ )
  {
    const String& keyName = m_Data[index].first;
    if ( keyName.CompareWithCase( lowerName ) == 0 )
    {
      return m_Data[index].second;
    }
  }
  return Variant::ZERO;
}

/***********************************************************************************
* GetInternalData:
* returns a value from the key (if it exist in this Bundle). If the key does not
* exist, a ZERO Variant value is returned. The name is always lower key.
* 
* @param  (const String&)  name - Name of the key
* @return (Variant)             - The value of the key
* 
* @author Thor Richard Hansen
***********************************************************************************/
Variant& Bundle::GetInternalData( const String& lowerName )
{
  for ( size_t index = 0; index < m_Data.size(); index++ )
  {
    const String& keyName = m_Data[index].first;
    if ( keyName.CompareWithCase( lowerName ) == 0 )
    {
      return m_Data[index].second;
    }
  }
  return Variant::ZERO;
}
