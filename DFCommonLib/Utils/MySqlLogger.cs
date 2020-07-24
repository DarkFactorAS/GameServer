using System;
using DFCommonLib.DataAccess;

namespace DFCommonLib.Utils
{
    public class MySqlLogger : IExternalLogger
    {
        IDbConnectionFactory _connection;

        public MySqlLogger(IDbConnectionFactory connection)
        {
            _connection = connection;
        }

        public void LogMessage(ExternalLogger.DFLogLevel logLevel, string group, string message)
        {
            var sql = @"insert into logtable (id,created, loglevel,group,message) values(0,sysdate, @loglevel,@group,@message)";
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
