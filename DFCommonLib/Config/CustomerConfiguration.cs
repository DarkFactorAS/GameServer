using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace DFCommonLib.Config
{
    public class CustomerConfiguration
    {
        public IEnumerable<Customer> Customers { get; set; }

        public Customer GetCustomer(string customerName)
        {
            return Customers.Where(x => x.Id == customerName).FirstOrDefault();
        }
    }

    public class Customer
    {
        public string Id { get; set; }
        public IEnumerable<DatabaseConnection> DatabaseConnections { get; set; }

        public DatabaseConnection GetDbConnection(string connectionType)
        {
            return DatabaseConnections.Where(x => x.ConnectionType == connectionType).FirstOrDefault();
        }
    }

    public class DatabaseConnection
    {
        public string ConnectionType { get; set; }
        public string ConnectionString { get; set; }
    }
}
