using System.IO;
using Microsoft.Extensions.Configuration;

namespace BGCommonLib.Config
{
    public class ConfigurationFactory
    {
        public ConfigurationFactory()
        {
            ConfigurationBuilder = GetConfigurationBuilder();
        }

        private IConfiguration GetConfigurationBuilder()
        {
            IConfiguration builder = new ConfigurationBuilder()
                .SetBasePath($"{Directory.GetCurrentDirectory()}/Config")
                .AddJsonFile(path: "appsettings.json", optional: false, reloadOnChange: true)
                .AddJsonFile(path: "customers.json", optional: false, reloadOnChange: true)
                //.AddJsonFile(path: "testsettings.json", optional: true, reloadOnChange: true)
                .Build();

            return builder;
        }

        public IConfiguration ConfigurationBuilder { get; }
    }
}
