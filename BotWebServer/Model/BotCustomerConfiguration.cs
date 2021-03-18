using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

using DFCommonLib.Config;

namespace BotWebServer.Model
{
    public class BotCustomerConfiguration
    {
        public IEnumerable<BotCustomer> Customers { get; set; }

        public BotCustomer GetCustomer(string customerName)
        {
            return Customers.Where(x => x.Id == customerName).FirstOrDefault();
        }
    }

    public class BotCustomer : Customer
    {
        public string AccountServer { get; set; }
    }
}
