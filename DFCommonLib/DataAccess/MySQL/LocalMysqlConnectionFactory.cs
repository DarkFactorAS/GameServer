using System;
using DFCommonLib.Config;
using DFCommonLib.Logger;

namespace DFCommonLib.DataAccess
{
    public class LocalMysqlConnectionFactory : MySQLDbConnectionFactory
    {
        public LocalMysqlConnectionFactory(IConfigurationHelper helper, IDFLogger<MySQLDbConnectionFactory> logger)
            : base("LOCAL", helper, logger )
        {
        }
    }
}
