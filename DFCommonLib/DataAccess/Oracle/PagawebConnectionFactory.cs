using BGCommonLib.Config;
using System;

namespace BGCommonLib.DataAccess
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
