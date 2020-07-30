using System;
using DFCommonLib.DataAccess;

namespace DFCommonLib.Logger
{
    public class MySqlLogWriter : ILogOutputWriter
    {
        IDbConnectionFactory _connection;

        public MySqlLogWriter(IDbConnectionFactory connection)
        {
            _connection = connection;
        }
        public string GetName()
        {
            return "MySqlLogWriter";
        }

        public void LogMessage(DFLogLevel logLevel, string group, string message)
        {
            var sql = @"insert into logtable (id,created, loglevel, groupname, message) values(0,sysdate(), @loglevel,@group,@message)";
            using (var command = _connection.CreateCommand(sql))
            {
                command.AddParameter("@loglevel", (int) logLevel);
                command.AddParameter("@group", group);
                command.AddParameter("@message", message);
                command.ExecuteNonQuery();
            }
        }
    }
}
