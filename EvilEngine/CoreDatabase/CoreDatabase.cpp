#include "Precompile.h"
#include "CoreDatabase.h"

// Ignore DLL warning in the mysql library

#if defined PLATFORM_WINDOWS
  #pragma warning( push )
    #pragma warning( disable : 4251)
    #include <mysql_driver.h>
    #include <mysql_connection.h>
    #include <cppconn/exception.h>
    #include <cppconn/resultset.h>
    #include <cppconn/statement.h>
    #include <cppconn/prepared_statement.h>
    #include <cppconn/sqlstring.h>
  #pragma warning( pop ) 
#else
  #include <mysql_driver.h>
  #include <mysql_connection.h>
  #include <cppconn/exception.h>
  #include <cppconn/resultset.h>
  #include <cppconn/statement.h>
  #include <cppconn/prepared_statement.h>
  #include <cppconn/sqlstring.h>
#endif

#include <string.h>
#include <istream>
#include <sstream>

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#define MAX_SQL_STRING_SIZE 16000

// Mysql errorcodes
// http://dev.mysql.com/doc/refman/5.7/en/error-messages-client.html
#define MYSQL_DISCONNECTED 2006
#define MYSQL_DISCONNECTED_WHILE_SELECT 2013
#define MYSQL_CONNECTIONREFUSED 2003

CoreDatabase* CoreDatabase::s_Instance = NULL;
bool CoreDatabase::s_Offline = false;
long CoreDatabase::s_Guard   = 0;
bool CoreDatabase::s_DBErrorAsFatal = true;

CoreDatabase::CoreDatabase() :
  m_Connection(NULL),
  m_DisplayFlags(0)
{
}

CoreDatabase::~CoreDatabase()
{
  Disconnect();
}

CoreDatabase* CoreDatabase::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new CoreDatabase();
  }
  return s_Instance;
}

void CoreDatabase::Destroy()
{
  delete s_Instance;
}

void CoreDatabase::FrameProcess(float deltaTime)
{
  if (m_Connection == NULL )
  {
    m_OfflineTimer -= deltaTime;
    if (m_OfflineTimer <= 0.0f)
    {
      m_OfflineTimer = 60.0f;
      Connect(m_Servername, m_Username, m_Password, m_Schema );
    }
  }
}

bool CoreDatabase::Connect( String serverAddress, String username, String password, String schema )
{
  if ( !s_Offline)
  {
    LogInfoFMT( "CoreDataBase", "Connecting to DB > '%s' ( %s ) ", schema.c_str(), serverAddress.c_str() );

    try
    {
      sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
      if (driver != NULL)
      {
        String mysqlServerAddress = String::FormatString("tcp://%s", serverAddress.c_str());

        //m_Servername = serverAddress;
        m_Servername = mysqlServerAddress;
        m_Username = username;
        m_Password = password;
        m_Schema = schema;

        m_Connection = driver->connect(m_Servername.c_str(), m_Username.c_str(), m_Password.c_str());
        if (m_Connection != NULL)
        {
          m_Connection->setSchema(m_Schema.c_str());
          m_Connection->setClientOption("collation_connection", "utf8");
          m_Connection->setClientOption("collation_database", "utf8");

          return true;
        }
        else
        {
          LogFatalFMT("CoreDatabase", "Failed to connect to server (Server %s)", serverAddress.c_str());
        }
      }
      else
      {
        LogFatalFMT("CoreDatabase", "Failed to load MySQL driver (Server %s)", serverAddress.c_str());
      }
    }
    catch (sql::SQLException &e)
    {
      ShowErrorCode(e, String::zero );
      m_Connection = NULL;
    }
  }
  return false;
}

void CoreDatabase::Disconnect()
{
  m_Connection = NULL;
  //delete m_Connection;
}

void CoreDatabase::ThrowDBException(sql::SQLException &e)
{
  if (CoreDatabase::s_DBErrorAsFatal)
  {
    dfAssertFMT(false, "CoreDatabase:Error (%d) '%s'. See log for more info.", e.getErrorCode(), e.what());
  }
  else
  {
    dfBugreportFMT(false, "CoreDatabase:Error (%d) '%s'. See log for more info.", e.getErrorCode(), e.what());
  }
}

void CoreDatabase::ShowErrorCode( sql::SQLException &e, String sqlString )
{
  const char* what = e.what();

  LogError   ( "CoreDataBase", "###### ERR: SQLException ###############################");
  LogErrorFMT( "CoreDataBase", "File: %s ", String(__FILE__).c_str() );
  LogErrorFMT( "CoreDataBase", "Func: %s ( line %d )", __FUNCTION__, __LINE__ );
  LogErrorFMT( "CoreDataBase", "SQL : %s", sqlString.c_str());

  if (what != NULL)
  {
    LogErrorFMT("CoreDataBase", "ERR : (code:%d) %s ( State : %s )", e.getErrorCode(), e.what(), e.getSQLState().c_str());
  }
  else
  {
    LogErrorFMT("CoreDataBase", "ERR : (code:%d) ( State : %s )", e.getErrorCode(), e.getSQLState().c_str());
  }

  LogError   ( "CoreDataBase", "########################################################");
}

//
// Select, returns the resultset
//
const SQLResultSet& CoreDatabase::ExecuteSelect( String sqlString, ... )
{
  va_list sArgList;
  va_start( sArgList, sqlString );
  const SQLResultSet& resultSet = ExecuteSelect( sqlString, sArgList );
  va_end( sArgList );
  return resultSet;
}

const SQLResultSet& CoreDatabase::ExecuteSelect( String sqlString, va_list pArgs )
{
  char pzBuffer[ MAX_SQL_STRING_SIZE ];
  memset( pzBuffer, 0, MAX_SQL_STRING_SIZE );

#ifdef WINDOWS
  vsnprintf_s( pzBuffer, sizeof( pzBuffer ), sizeof( pzBuffer ) / sizeof( pzBuffer[0] ), sqlString.c_str(), pArgs );
#else
  vsprintf( pzBuffer, sqlString.c_str(), pArgs );
#endif

  return ExecuteInternalSelect( pzBuffer );
}

const SQLResultSet& CoreDatabase::ExecuteInternalSelect( String sqlString )
{
  if ( sqlString.empty() )
  {
    dfBugreport(false,"Database:Error Empty sql string" );
    return *( new SQLResultSet( NULL ) );
  }

  sql::ResultSet *resultSet = NULL;
  if ( !s_Offline )
  {
    sql::Statement *statement = NULL;
    try 
    {
      if ( ShowSelectDebug() )
      {
        LogInfoFMT("CoreDataBase", "## SQL ## %s", sqlString.c_str());
      }
      if (m_Connection != NULL)
      {
        statement = m_Connection->createStatement();
        resultSet = statement->executeQuery(sqlString.c_str());
        delete statement;
      }
      else
      {
        LogFatalFMT("CoreDataBase", "## No connection object ## (SQL:%s)", sqlString.c_str());
      }
    }
    catch (sql::SQLException &e) 
    {
      ShowErrorCode( e, sqlString );

      delete statement;
      switch( e.getErrorCode() )
      {
        // MysqlServer disconnected
        case MYSQL_DISCONNECTED:
        case MYSQL_DISCONNECTED_WHILE_SELECT:
        {
            // Try to reconnect ONCE and rerun the query
            if ( !s_Guard && Connect( m_Servername, m_Username, m_Password, m_Schema ) )
            {
              s_Guard = true;
              const SQLResultSet& sqlResultSet = ExecuteInternalSelect( sqlString );
              s_Guard = false;
              return sqlResultSet;
            }
          }
          break;

        default:
          ThrowDBException(e);
          break;
      }
    }
  }
  return *( new SQLResultSet( resultSet ) );
}

//
// Delete/Insert, returns bool
//
int CoreDatabase::ExecuteDelete( String sqlString, ... )
{
  va_list sArgList;
  va_start( sArgList, sqlString );
  int ret = ExecuteDelete( sqlString, sArgList );
  va_end( sArgList );
  return ret;
}

/************************************************************************************************
* ExecuteDelete:
* Delete records in the database. This function will return the number of rows deleted. If the
* query is a select, it will return -1 as error.
*
* @param  (String)  sqlString   - The db query to execute
* @param  (va_list) pArgs       - Arguments for the sql string
* @return (int)                 - Return the number of deleted rows
*
* @author Thor Richard Hansen
*************************************************************************************************/
int CoreDatabase::ExecuteDelete( String sqlString, va_list pArgs )
{
  char pzBuffer[ MAX_SQL_STRING_SIZE ];
  memset( pzBuffer, 0, MAX_SQL_STRING_SIZE );

#ifdef WINDOWS
  vsnprintf_s( pzBuffer, sizeof( pzBuffer ), sizeof( pzBuffer ) / sizeof( pzBuffer[0] ), sqlString.c_str(), pArgs );
#else
  vsprintf( pzBuffer, sqlString.c_str(), pArgs );
#endif

  return ExecuteInternalDelete( pzBuffer );
}

bool CoreDatabase::ExecuteInsert( String sqlString, ... )
{
  va_list sArgList;
  va_start( sArgList, sqlString );
  bool ret = ExecuteInsert( sqlString, sArgList );
  va_end( sArgList );
  return ret;
}

bool CoreDatabase::ExecuteInsert( String sqlString, va_list pArgs )
{
  char pzBuffer[ MAX_SQL_STRING_SIZE ];
  memset( pzBuffer, 0, MAX_SQL_STRING_SIZE );

#ifdef WINDOWS
  vsnprintf_s( pzBuffer, sizeof( pzBuffer ), sizeof( pzBuffer ) / sizeof( pzBuffer[0] ), sqlString.c_str(), pArgs );
#else
  vsprintf( pzBuffer, sqlString.c_str(), pArgs );
#endif

  return ExecuteInternalInsert( pzBuffer );
}

bool CoreDatabase::ExecuteInternalInsert( String sqlString )
{
  if ( sqlString.empty() )
  {
    dfBugreport(false,"Database:Error Empty sql string" );
    return false;
  }

  if ( !s_Offline )
  {
    sql::Statement *statement = NULL;
    try 
    {
      if (ShowInsertDebug())
      {
        LogInfoFMT("CoreDataBase", "## SQL ## %s", sqlString.c_str());
      }
      if ( m_Connection != NULL)
      {
        statement = m_Connection->createStatement();
        statement->execute(sqlString.c_str());
        delete statement;
        return true;
      }
      else
      {
        LogFatalFMT("CoreDataBase", "## No connection object ## (SQL:%s)", sqlString.c_str());
        return false;
      }
    }
    catch (sql::SQLException &e) 
    {
      ShowErrorCode( e, sqlString );
      delete statement;
      switch( e.getErrorCode() )
      {
        case MYSQL_DISCONNECTED: // MysqlServer disconnected
        case MYSQL_DISCONNECTED_WHILE_SELECT:
          {
              if ( !s_Guard && Connect( m_Servername, m_Username, m_Password, m_Schema ) )
              {
                s_Guard = true;
                bool ret = ExecuteInternalInsert( sqlString );
                s_Guard = false;
                return ret;
              }
            }
            break;

        default:
          ThrowDBException(e);
          return false;
      }
    }
  }
  return s_Offline;
}


int CoreDatabase::ExecuteUpdate( String sqlString, ... )
{
  va_list sArgList;
  va_start( sArgList, sqlString );
  int ret = ExecuteUpdate( sqlString, sArgList );
  va_end( sArgList );
  return ret;
}
int CoreDatabase::ExecuteUpdate( String sqlString, va_list pArgs )
{
  char pzBuffer[ MAX_SQL_STRING_SIZE ];
  memset( pzBuffer, 0, MAX_SQL_STRING_SIZE );

#ifdef WINDOWS
  vsnprintf_s( pzBuffer, sizeof( pzBuffer ), sizeof( pzBuffer ) / sizeof( pzBuffer[0] ), sqlString.c_str(), pArgs );
#else
  vsprintf( pzBuffer, sqlString.c_str(), pArgs );
#endif

  return ExecuteInternalUpdate( pzBuffer );
}

int CoreDatabase::ExecuteInternalUpdate( String sqlString )
{
  if ( sqlString.empty() )
  {
    dfBugreport(false,"Database:Error Empty sql string" );
    return 0;
  }

  if ( !s_Offline )
  {
    sql::Statement *statement = NULL;
    try 
    {
      if (ShowUpdateDebug())
      {
        LogInfoFMT("CoreDataBase", "## SQL ## %s", sqlString.c_str());
      }

      if ( m_Connection != NULL )
      {
        statement = m_Connection->createStatement();
        int numUpdatedRows = statement->executeUpdate(sqlString.c_str());
        delete statement;
        return numUpdatedRows;
      }
      else
      {
        LogFatalFMT("CoreDataBase", "## No connection object ## (SQL:%s)", sqlString.c_str());
        return 0;
      }
    }
    catch (sql::SQLException &e) 
    {
      ShowErrorCode( e, sqlString );
      delete statement;
      switch( e.getErrorCode() )
      {
        case MYSQL_DISCONNECTED: // MysqlServer disconnected
        case MYSQL_DISCONNECTED_WHILE_SELECT:
          {
            if ( !s_Guard && Connect( m_Servername, m_Username, m_Password, m_Schema ) )
            {
              s_Guard = true;
              int ret = ExecuteInternalUpdate( sqlString );
              s_Guard = false;
              return ret;
            }
          }
          break;

        default:
          ThrowDBException(e);
          break;
      }
    }
  }
  return ( s_Offline ) ? 1 : 0;
}

/************************************************************************************************
* ExecuteBlobUpdate:
* Update the database with a BLOB field. the blob will be streamed to the MySQL fieldin the table.
*
* @param  (String) sqlString   - The db query to execute
* @param  (uint32) bitText     - contents of the blob
* @return (int)                - Return the number of updated rows
*
* @author Thor Richard Hansen
*************************************************************************************************/
int CoreDatabase::ExecuteBlobUpdate(String sqlString, const char* bigText )
{
  if (sqlString.empty() || bigText == NULL )
  {
    dfAssert(false, "Database:Error Empty sql string");
    return 0;
  }

  if (!s_Offline)
  {
    sql::PreparedStatement *statement = NULL;
    try
    {
      if (ShowUpdateDebug())
      {
        LogInfoFMT("CoreDataBase", "## SQL ## %s", sqlString.c_str());
      }

      if ( m_Connection != NULL )
      {
        statement = m_Connection->prepareStatement(sqlString.c_str());
        statement->clearParameters();
        const sql::SQLString& value(bigText);
        statement->setString(1, value);

        int updatedRows = statement->executeUpdate();

        delete statement;
        //delete bigText;
        return updatedRows;
      }
      else
      {
        LogFatalFMT("CoreDataBase", "## No connection object ## (SQL:%s)", sqlString.c_str());
        return 0;
      }
    }
    catch (sql::SQLException &e)
    {
      ShowErrorCode(e, sqlString);
      delete statement;
      switch (e.getErrorCode())
      {
        case MYSQL_DISCONNECTED: // MysqlServer disconnected
        case MYSQL_DISCONNECTED_WHILE_SELECT:
          {
            if (!s_Guard && Connect(m_Servername, m_Username, m_Password, m_Schema))
            {
              s_Guard = true;
              int ret = ExecuteBlobUpdate(sqlString, bigText);
              s_Guard = false;
              return ret;
            }
          }
          break;

        default:
          ThrowDBException(e);
          break;
      }
    }
  }
  return (s_Offline) ? 1 : 0;
}


/************************************************************************************************
* ExecuteInternalDelete:
* Delete records in the database. This function will return the number of rows deleted. If the 
* query is a select, it will return -1 as error.
*
* @param  (String) sqlString   - The db query to execute
* @return (int)                - Return the number of deleted rows
*
* @author Thor Richard Hansen
*************************************************************************************************/
int CoreDatabase::ExecuteInternalDelete(String sqlString)
{
  if (sqlString.empty())
  {
    dfAssert(false, "Database:Error Empty sql string");
    return -1;
  }

  if (!s_Offline)
  {
    sql::Statement *statement = NULL;
    try
    {
      if (ShowDeleteDebug())
      {
        LogInfoFMT("CoreDataBase", "## SQL ## %s", sqlString.c_str());
      }

      int numDeletedRows = -1;
      if (m_Connection != NULL)
      {
        statement = m_Connection->createStatement();
        bool wasSelect = statement->execute(sqlString.c_str());
        if ( !wasSelect )
        {
          numDeletedRows = (int)statement->getUpdateCount();
        }
        delete statement;
        return numDeletedRows;
      }
      else
      {
        LogFatalFMT("CoreDataBase", "## No connection object ## (SQL:%s)", sqlString.c_str());
        return 0;
      }
    }
    catch (sql::SQLException &e)
    {
      ShowErrorCode(e, sqlString);
      delete statement;
      switch (e.getErrorCode())
      {
        case MYSQL_DISCONNECTED: // MysqlServer disconnected
        case MYSQL_DISCONNECTED_WHILE_SELECT:
        {
          if (!s_Guard && Connect(m_Servername, m_Username, m_Password, m_Schema))
          {
            s_Guard = true;
            int ret = ExecuteInternalDelete(sqlString);
            s_Guard = false;
            return ret;
          }
        }
        break;

      default:
        ThrowDBException(e);
        break;
      }
    }
  }
  return 0;
}

const char* CoreDatabase::SQLTrunk(const String& text, uint32 maxLength)
{
  if (text.length() > maxLength)
  {
    char* outTxt = new char[maxLength];
#ifdef PLATFORM_WINDOWS
    strncpy_s(outTxt, maxLength, text.c_str(), maxLength);
#else
    strncpy(outTxt, text.c_str(), maxLength);
#endif
    return outTxt;
  }

  return text.c_str();
}

const char* CoreDatabase::SQLFormat( const String& text, uint32 maxLength )
{
  if ( text.length() == 0 )
  {
    return text.c_str();
  }

  uint32 strLength = text.length() + 1;
  if ( strLength > maxLength && maxLength > 0 )
  {
    strLength = maxLength;
  }

  char* outTxt = new char[ strLength ];
  memset( outTxt, 0, strLength );
  for ( uint index = 0; index < strLength - 1; index++ )
  {
    char c = text.CharacterAt( index );

    // '
    if (c == 96)
    {
      outTxt[index] = '-';
    }
    // 0-9 and A-Z and a-z
    if (c >= 48 && c <= 122)
    {
      outTxt[index] = c;
    }
    // Space
    else if (c == ' ')
    {
      outTxt[index] = c;
    }
    else
    {
      outTxt[index] = '-';
    }

    //switch ( c )
    //{
    //  case '\'': c = ' ';
    //  case '\"': c = ' ';
    //  case '\\': c = '/';
    //    break;

    //  case '0':
    //  case '1':
    //  case '2':
    //  case '3':
    //  case '4':
    //  case '5':
    //  case '6':
    //  case '7':
    //  case '8':
    //  case '9':
    //    break;

    //  default:
    //    break;
    //}
    //outTxt[ index ] = c;
  }
  return outTxt;
}

bool CoreDatabase::ShowSelectDebug()
{
  return ( ( m_DisplayFlags & DISPLAYFLAG_SELECT ) == DISPLAYFLAG_SELECT );
}
bool CoreDatabase::ShowUpdateDebug()
{
  return ((m_DisplayFlags & DISPLAYFLAG_UPDATE) == DISPLAYFLAG_UPDATE);
}
bool CoreDatabase::ShowInsertDebug()
{
  return ((m_DisplayFlags & DISPLAYFLAG_INSERT) == DISPLAYFLAG_INSERT);
}
bool CoreDatabase::ShowDeleteDebug()
{
  return ((m_DisplayFlags & DISPLAYFLAG_DELETE) == DISPLAYFLAG_DELETE);
}
