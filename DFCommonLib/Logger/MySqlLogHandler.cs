using System;
using DFCommonLib.DataAccess;

namespace DFCommonLib.Logger
{
    public class MySqlLogHandler : ILogOuputHandler
    {
        IDbConnectionFactory _connection;

        public MySqlLogHandler(IDbConnectionFactory connection)
        {
            _connection = connection;
        }

        public void LogMessage(OutputHandler.DFLogLevel logLevel, string group, string message)
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
