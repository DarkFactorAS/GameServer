using System;
using System.Data;
using System.Linq;
using DFCommonLib.Config;
using DFCommonLib.Logger;

namespace DFCommonLib.DataAccess
{
    public abstract class MySQLDbConnectionFactory : IDbConnectionFactory
    {
        private readonly string _connectionType;

        private string _connectionString;
        private IConfigurationHelper _helper;
        private IDFLogger<MySQLDbConnectionFactory> _logger;

        public MySQLDbConnectionFactory(string connectionType, IConfigurationHelper helper, IDFLogger<MySQLDbConnectionFactory> logger )
        {
            _connectionType = connectionType;
            _helper = helper;
            _logger = logger;
        }

        public IDbConnection CreateConnection()
        {
            string connectionString = GetConnectionString();
            return new MySQLDbConnection(connectionString);
        }

        public IBluDbCommand CreateCommand(string commandText)
        {
            var connection = CreateConnection();
            return new TimedMySQLDbCommand(commandText, connection as MySQLDbConnection, true);
        }

        public IBluDbCommand CreateCommand(string commandText, IDbConnection connection)
        {
            return new TimedMySQLDbCommand(commandText, connection as MySQLDbConnection, false);
        }

        private string GetConnectionString()
        {
            if (_connectionString == null)
            {
                if (_helper == null)
                {
                    throw new Exception("DB helper returned NULL, make sure customer has a connection in the config");
                }

                var customer = _helper.GetFirstCustomer();
                if (customer == null)
                {
                    throw new Exception("DB customer returned NULL, make sure customer has a connection in the config");
                }

                var configDbConnection = customer.GetDbConnection(_connectionType);
                if (configDbConnection == null)
                {
                    throw new Exception("DB connection returned NULL, make sure customer has a connection in the config");
                }

                _connectionString = configDbConnection.ConnectionString;

                _logger.LogInfo(string.Format("Connection string: {0} / {1}", _connectionType, _connectionString));
            }
            return _connectionString;
        }
    }

}
