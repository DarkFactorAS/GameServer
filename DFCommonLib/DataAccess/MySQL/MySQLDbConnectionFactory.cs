using System;
using System.Data;
using DFCommonLib.Config;

namespace DFCommonLib.DataAccess
{
    public abstract class MySQLDbConnectionFactory
    {
        private readonly string _connectionType;

        private string _connectionString;
        private Customer _customer;

        public MySQLDbConnectionFactory(string connectionType, Customer customer)
        {
            _connectionType = connectionType;
            _customer = customer;
        }

        public MySQLDbConnection CreateConnection()
        {
            return new MySQLDbConnection(GetConnectionString());
        }

        public IBluDbCommand CreateCommand(string commandText)
        {
            return new TimedMySQLDbCommand(commandText, CreateConnection(), true);
        }

        public IBluDbCommand CreateCommand(string commandText, MySQLDbConnection connection)
        {
            return new TimedMySQLDbCommand(commandText, connection, false);
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
