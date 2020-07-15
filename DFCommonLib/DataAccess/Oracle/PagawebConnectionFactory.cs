using System;
using DFCommonLib.Config;

namespace DFCommonLib.DataAccess
{
    public interface IPagawebConnectionFactory : IDbConnectionFactory
    {
    }

    public class PagawebConnectionFactory : OracleDbConnectionFactory, IPagawebConnectionFactory
    {
        public PagawebConnectionFactory(Customer customer) :
            base("PAGAWEB", customer)
        {
        }
    }
}
