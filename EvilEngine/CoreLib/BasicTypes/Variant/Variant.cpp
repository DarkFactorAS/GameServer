
#include "Precompile.h"
#include "Variant.h"

Variant Variant::ZERO;

Variant::Variant() :
  m_IntValue( 0 ),
  m_FloatValue( 0.0f ),
  m_Type( NONE )
{
}

Variant::Variant( uint32 val ) :
  m_IntValue( val ),
  m_FloatValue( 0.0f ),
  m_Type( INT )
{
}

Variant::Variant(String val) :
  m_IntValue(0),
  m_FloatValue(0.0f),
  m_StringValue( val ),
  m_Type(STRING)
{
}

void Variant::Copy( const Variant& other )
{
  m_Type        = other.m_Type;
  m_IntValue    = other.m_IntValue;
  m_FloatValue  = other.m_FloatValue;
  m_StringValue = other.m_StringValue;
}

void Variant::SetShort( short fieldValue )
{
  m_Type        = INT;
  m_IntValue    = fieldValue;
}

void Variant::SetUInt32( uint32 fieldValue )
{
  m_Type        = INT;
  m_IntValue    = fieldValue;
}

void Variant::SetInt32( int32 fieldValue )
{
  m_Type        = INT;
  m_IntValue    = (uint32)fieldValue;
}

void Variant::SetFloat( float fieldValue )
{
  m_Type        = FLOAT;
  m_FloatValue  = fieldValue;
}

void Variant::SetBool( bool fieldValue )
{
  m_Type        = BOOLVALUE;
  m_IntValue    = ( fieldValue ) ? 1 : 0;
}

void Variant::SetString( const String& stringValue )
{
  m_Type        = STRING;
  m_StringValue = stringValue;
}

uint32 Variant::AsUInt32() const
{
  switch ( m_Type )
  {
    case INT:
      return m_IntValue;
    case FLOAT:
      return (uint32)m_FloatValue;
    case BOOLVALUE:
    case SHORT:
      return (uint32) m_IntValue;
    case STRING:
      {
        return atoi( m_StringValue.c_str() );
      }  
    default:
      break;
  }
  return 0;
}

String Variant::ToString()
{
  String str = "";
  switch( m_Type )
  {
    case INT:
      str = String::FormatString( "%d", m_IntValue );
      break;
    case FLOAT:
      str = String::FormatString( "0.2f", m_FloatValue );
      break;
    case SHORT:
      str = String::FormatString( "%d", m_IntValue );
      break;
    case BOOLVALUE:
      str = String::FormatString( "%s", (m_IntValue==1) ? "true" : "false" );
      break;
    case STRING:
      str = m_StringValue;
      break;
    case NONE:
      str = "?";
      break;
  }
  return str;
}
