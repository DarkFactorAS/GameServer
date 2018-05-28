#ifndef EVILENGINE_COREDATABASE_COREDATABASE_H
#define EVILENGINE_COREDATABASE_COREDATABASE_H

#include "EvilEngine/CoreDatabase/SQLResult.h"
#include <stdarg.h>
#include <streambuf>

namespace sql
{
  class Connection;
  class SQLException;
  class ResultSet;
}

class membuf : public std::basic_streambuf<char>
{
public:
  membuf(char* p, size_t n) {
    setg(p, p, p + n);
    setp(p, p + n);
  }
};

class CoreDatabase
{
public:

  enum DISPLAYFLAGS
  {
    DISPLAYFLAG_NONE    = 0,
    DISPLAYFLAG_SELECT  = 1<<0,
    DISPLAYFLAG_UPDATE = 1 << 1,
    DISPLAYFLAG_INSERT = 1 << 2,
    DISPLAYFLAG_DELETE = 1 << 3,
    DISPLAYFLAG_ALL    = DISPLAYFLAG_SELECT | DISPLAYFLAG_UPDATE | DISPLAYFLAG_INSERT | DISPLAYFLAG_DELETE,
  };

  CoreDatabase();
  ~CoreDatabase();

  static CoreDatabase*  s_Instance;
  static CoreDatabase*  GetInstance();
  static void           Destroy();
  static void           SetOfflineMode(){ s_Offline = true; }
  static bool           IsInOfflineMode(){ return s_Offline; }

  bool                  Connect( String serverAddress, String username, String passowrd, String schema );
  void                  Disconnect();

  const SQLResultSet&   ExecuteSelect( String sqlString, ... );
  const SQLResultSet&   ExecuteSelect( String sqlString, va_list pArgs );

  bool                  ExecuteInsert( String sqlString, ... );
  bool                  ExecuteInsert( String sqlString, va_list pArgs );

  int                   ExecuteUpdate( String sqlString, ... );
  int                   ExecuteUpdate( String sqlString, va_list pArgs );
  int                   ExecuteBlobUpdate(String sqlString, const char* bigText );

  int                   ExecuteDelete( String sqlString, ... );
  int                   ExecuteDelete( String sqlString, va_list pArgs );

  static const char*    SQLTrunk(const String& text, uint32 maxLength);
  static const char*    SQLFormat( const String& text, uint32 maxLength = 0 );

  void                  SetDatabaseDisplayFlags( uint32 flags ){ m_DisplayFlags = flags; }
  static void           NotCrashServerOnDbError(){ s_DBErrorAsFatal = false; }
private:

  void                  ThrowDBException(sql::SQLException &e);

  void                  ShowErrorCode( sql::SQLException &e, String sqlString );

  const SQLResultSet&   ExecuteInternalSelect( String sqlString );
  bool                  ExecuteInternalInsert( String sqlString );
  int                   ExecuteInternalUpdate( String sqlString );
  int                   ExecuteInternalDelete( String sqlString );

  bool                  ShowSelectDebug();
  bool                  ShowInsertDebug();
  bool                  ShowUpdateDebug();
  bool                  ShowDeleteDebug();

  static bool           s_Offline;
  static long           s_Guard;
  static bool           s_DBErrorAsFatal;

  String                m_Servername;
  String                m_Username;
  String                m_Password;
  String                m_Schema;
  sql::Connection*      m_Connection;
  uint32                m_DisplayFlags;
};

#endif /// EVILENGINE_COREDATABASE_COREDATABASE_H
