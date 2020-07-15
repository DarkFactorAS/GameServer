using System;
using DFCommonLib.Config;

namespace DFCommonLib.DataAccess
{
    public class LocalMysqlConnectionFactory : MySQLDbConnectionFactory
    {
        public LocalMysqlConnectionFactory(Customer customer)
            : base("LOCAL", customer )
        {
        }
    }
}
