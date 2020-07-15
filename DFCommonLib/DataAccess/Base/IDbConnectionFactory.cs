using System;
using System.Data;

namespace DFCommonLib.DataAccess
{
    public interface IDbConnectionFactory
    {
        IDbConnection CreateConnection();
        IBluDbCommand CreateCommand(string commandText);
        IBluDbCommand CreateCommand(string commandText, IDbConnection connection);
    }
}