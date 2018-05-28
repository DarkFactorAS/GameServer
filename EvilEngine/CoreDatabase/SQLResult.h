#ifndef EVILENGINE_COREDATABASE_SQLRESULTSET_H
#define EVILENGINE_COREDATABASE_SQLRESULTSET_H

namespace sql
{
  class ResultSet;
}

class SQLResultSet
{
public:
  SQLResultSet();
  SQLResultSet( sql::ResultSet* );
  ~SQLResultSet();

  void    Close();
  bool    Next() const;

  String  GetString( int index = -1 ) const;
  int32   GetInt( int index = -1 ) const;
  uint8   GetUInt8(int index = -1 ) const;
  uint16  GetUInt16(int index = -1) const;
  uint32  GetUInt32( int index = -1 ) const;

private:

  mutable int     m_Index;
  sql::ResultSet* m_ResultSet;
};

#endif /// EVILENGINE_COREDATABASE_SQLRESULTSET_H
