using System.IO;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;

namespace DFCommonLib.Config
{
    public class ConfigurationFactory
    {
        IHostEnvironment _env;

        public ConfigurationFactory(IHostEnvironment env)
        {
            _env = env;
            ConfigurationBuilder = GetConfigurationBuilder();
        }

        private IConfiguration GetConfigurationBuilder()
        {
            string customerConfig = "customers.json";
            if ( _env.IsDevelopment() )
            {
                customerConfig = "customers_dev.json";
            }

            IConfiguration config = new ConfigurationBuilder()
                .SetBasePath($"{Directory.GetCurrentDirectory()}/Config")
                .AddJsonFile(path: "appsettings.json", optional: false, reloadOnChange: true)
                .AddJsonFile(path: customerConfig, optional: false, reloadOnChange: true)
                //.AddJsonFile(path: "testsettings.json", optional: true, reloadOnChange: true)
                .Build();

            return config;
        }

        public IConfiguration ConfigurationBuilder { get; }
    }
}
