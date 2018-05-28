#include "Precompile.h"
#include "SQLResult.h"
#include <string.h>

// Ignore warnings in the mysql library
#if defined PLATFORM_WINDOWS
  #pragma warning( push )
    #pragma warning( disable : 4251)
    #include <cppconn/resultset.h>
    #include <cppconn/statement.h>
  #pragma warning( pop ) 
#else
  #include <cppconn/resultset.h>
  #include <cppconn/statement.h>
#endif


SQLResultSet::SQLResultSet()
{
  m_Index = 1;
  m_ResultSet = NULL;
}

SQLResultSet::SQLResultSet( sql::ResultSet* resultSet )
{
  m_Index     = 1;
  m_ResultSet = resultSet;
}

SQLResultSet::~SQLResultSet()
{
  Close();
}

void SQLResultSet::Close()
{
  if ( m_ResultSet != NULL )
  {
    const sql::Statement* stm = m_ResultSet->getStatement();
    delete m_ResultSet;
    delete stm;
    m_ResultSet = NULL;
  }
}

bool SQLResultSet::Next() const
{
  m_Index = 1;
  if ( m_ResultSet != NULL )
  {
    return m_ResultSet->next();
  }
  return false;
}

int32 SQLResultSet::GetInt( int index ) const
{
  if ( m_ResultSet != NULL )
  {
    if ( index < 0 )
    {
      return m_ResultSet->getInt(m_Index++);
    }
    else
    {
      return m_ResultSet->getInt( index );
    }
  }
  return 0;
}

uint32 SQLResultSet::GetUInt32( int index ) const
{
  return GetInt( index );
}

uint16 SQLResultSet::GetUInt16( int index ) const
{
  if ( m_ResultSet != NULL )
  {
    if ( index < 0 )
    {
      return (uint16)m_ResultSet->getUInt(m_Index++);
    }
    else
    {
      return (uint16)m_ResultSet->getUInt(index);
    }
  }
  return 0;
}

uint8 SQLResultSet::GetUInt8(int index) const
{
  return (uint8) GetUInt16( index );
}

String SQLResultSet::GetString( int index ) const
{
  if ( m_ResultSet != NULL )
  {
    if ( index < 0 )
    {
      sql::SQLString str = m_ResultSet->getString(m_Index++);
      return str.c_str();
    }
    else
    {
      sql::SQLString str = m_ResultSet->getString(index);
      return str.c_str();
    }
  }
  return String::zero;
}

