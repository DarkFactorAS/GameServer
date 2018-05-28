#ifndef EVILENGINE_CORELIB_BASICTYPES_BUNDLE
#define EVILENGINE_CORELIB_BASICTYPES_BUNDLE

#include "../Variant/Variant.h"
#include "../String/CoreString.h"
#include <stdlib.h>
#include <vector>

class Bundle
{
public:

  void              AddData( const String& name, Variant val );
  const Variant&    GetData( const String& name ) const;

  void              Clear(){ m_Data.clear(); }

private:

  const Variant&    GetInternalData( const String& lowerName ) const;
  Variant&          GetInternalData( const String& lowerName );

  std::vector< std::pair< String, Variant > > m_Data;
};

#endif /// EVILENGINE_CORELIB_BASICTYPES_BUNDLE
