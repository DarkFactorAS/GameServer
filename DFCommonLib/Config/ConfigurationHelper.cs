using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Configuration;

namespace DFCommonLib .Config
{
    public interface IConfigurationHelper
    {
        ConfigurationSettings ConfigurationSettings { get; }
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

        public ConfigurationHelper()
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
