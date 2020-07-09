using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BGCommonLib.DataAccess
{
    public class DbPoolElement
    {
        public string customerName;
        public IDbConnectionFactory connection;
    }

    public class DbConnectionPool
    {
        List<DbPoolElement> connectionList;

        public DbConnectionPool()
        {
            connectionList = new List<DbPoolElement>();
        }

        public IDbConnectionFactory GetConnection(string customerName)
        {
            var element = connectionList.Where(x => x.customerName == customerName).FirstOrDefault();
            if ( element != null )
            {
                return element.connection;
            }
            return null;
        }

        public void AddConnection(string customerName, IDbConnectionFactory connection )
        {
            var element = connectionList.Where(x => x.customerName == customerName).FirstOrDefault();
            if ( element == null )
            {
                connectionList.Add(new DbPoolElement()
                {
                    customerName = customerName,
                    connection = connection
                });
            }
        }
    }
}
