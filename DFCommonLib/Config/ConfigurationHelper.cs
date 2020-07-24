using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;

namespace DFCommonLib .Config
{
    public interface IConfigurationHelper
    {
        ConfigurationSettings ConfigurationSettings { get; }
        Customer GetFirstCustomer();
    }

    public class ConfigurationHelper : ConfigurationFactory, IConfigurationHelper
    {
        public static ConfigurationSettings _configSettings;

        public ConfigurationSettings ConfigurationSettings
        {
            get
            {
                return _configSettings;
            }
        }

        public Customer GetFirstCustomer()
        {
            if (_configSettings != null )
            {
                var customerSetting = _configSettings.CustomerSettings;
                if ( customerSetting != null )
                {
                    var customers = customerSetting.Customers;
                    if ( customers != null )
                    {
                        return customers.FirstOrDefault();
                    }
                }
            }
            return null;
        }

        public ConfigurationHelper(IHostEnvironment env) : base(env)
        {
            if (_configSettings == null)
            {
                _configSettings = GetConfigurationFromBuilder(ConfigurationBuilder);
            }
        }

        private ConfigurationSettings GetConfigurationFromBuilder(IConfiguration builder)
        {
            var configSettings = new ConfigurationSettings();
            builder.Bind(configSettings);
            return configSettings;
        }
    }
}
