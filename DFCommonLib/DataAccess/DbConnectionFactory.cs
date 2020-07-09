using System;
using BGCommonLib.Config;

namespace BGCommonLib.DataAccess
{
    public interface IDbConnectionFactory
    {
        DbConnection CreateConnection();
        IBluDbCommand CreateCommand(string commandText);
        IBluDbCommand CreateCommand(string commandText, DbConnection connection);
    }

    public abstract class DbConnectionFactory : IDbConnectionFactory
    {
        private readonly string _connectionType;

        private string _connectionString;
        private Customer _customer;

        public DbConnectionFactory(string connectionType, Customer customer)
        {
            _connectionType = connectionType;
            _customer = customer;
        }

        public DbConnection CreateConnection()
        {
            return new DbConnection(GetConnectionString());
        }

        public IBluDbCommand CreateCommand(string commandText)
        {
            return new TimedDbCommand(commandText, CreateConnection(), true);
        }

        public IBluDbCommand CreateCommand(string commandText, DbConnection connection)
        {
            return new TimedDbCommand(commandText, connection, false);
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
