using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;

using BotWebServer.Model;
using DFCommonLib.Config;

namespace BotWebServer.Provider
{
    public class BotConfigurationHelper : ConfigurationFactory, IConfigurationHelper
    {
        public BotConfigurationSettings _configSettings;

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

        public BotConfigurationHelper(IHostEnvironment env) : base(env)
        {
            if (_configSettings == null)
            {
                _configSettings = GetConfigurationFromBuilder(ConfigurationBuilder);
            }
        }

        protected BotConfigurationSettings GetConfigurationFromBuilder(IConfiguration builder)
        {
            var configSettings = new BotConfigurationSettings();
            builder.Bind(configSettings);
            return configSettings;
        }
    }
}