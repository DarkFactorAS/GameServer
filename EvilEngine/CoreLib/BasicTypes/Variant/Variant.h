
#ifndef EVILENGINE_CORELIB_VARIANT
#define EVILENGINE_CORELIB_VARIANT 1

class Variant
{
public:

  enum TYPE
  {
    NONE,
    INT,
    FLOAT,
    STRING,
    SHORT,
    BOOLVALUE,
  };

  Variant();
  Variant( uint32 val );
  Variant( String val);

  void                        Copy( const Variant& other );

  uint32                      GetUInt32() const { return m_IntValue; }
  int32                       GetInt32() const { return (int32)m_IntValue; }
  float                       GetFloat() const { return m_FloatValue; }
  short                       GetShort() const{ return (short)m_IntValue; }
  const String&               GetString() const { return m_StringValue; }
  bool                        GetBool() const { return ( m_IntValue != 0 ); }

  void                        SetUInt32( uint32 fieldValue );
  void                        SetInt32( int32 fieldValue );
  void                        SetFloat( float fieldValue );
  void                        SetString( const String& stringValue );
  void                        SetShort( short fieldValue );
  void                        SetBool( bool fieldValue );

  uint32                      AsUInt32() const;


  TYPE                        GetType() const { return m_Type; }

  String                      ToString();

  bool                        IsNull() const { return m_Type == NONE; }
  static Variant              ZERO;

private:

  uint32    m_IntValue;
  float     m_FloatValue;
  String    m_StringValue;
  TYPE      m_Type;

};

#endif /// EVILENGINE_CORELIB_VARIANT