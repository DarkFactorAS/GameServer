using System.IO;
using Microsoft.Extensions.Configuration;

namespace DFCommonLib.Config
{
    public class ConfigurationFactory
    {
        public ConfigurationFactory()
        {
            ConfigurationBuilder = GetConfigurationBuilder();
        }

        private IConfiguration GetConfigurationBuilder()
        {
            string customerConfig = "customers.json";
#if DEBUG
            customerConfig = "customers_dev.json";
#endif

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
