using System;
using System.Data;
using DFCommonLib.Config;

namespace DFCommonLib.DataAccess
{
    public abstract class OracleDbConnectionFactory : IDbConnectionFactory
    {
        private readonly string _connectionType;

        private string _connectionString;
        private Customer _customer;

        public OracleDbConnectionFactory(string connectionType, Customer customer)
        {
            _connectionType = connectionType;
            _customer = customer;
        }

        public IDbConnection CreateConnection()
        {
            return new OracleDbConnection(GetConnectionString());
        }

        public IBluDbCommand CreateCommand(string commandText)
        {
            var connection = CreateConnection();
            return new TimedOracleDbCommand(commandText, connection as OracleDbConnection, true);
        }

        public IBluDbCommand CreateCommand(string commandText, IDbConnection connection)
        {
            return new TimedOracleDbCommand(commandText, connection as OracleDbConnection, false);
        }

        private string GetConnectionString()
        {
            if (_connectionString == null)
            {
                if (_customer == null)
                {
                    throw new Exception("DB customer returned NULL, make sure customer has a connection in the config");
                }
                var configDbConnection = _customer.GetDbConnection(_connectionType);
                _connectionString = configDbConnection.ConnectionString;
            }
            return _connectionString;
        }
    }

}
