using System;
using System.Collections.Generic;
using System.Text;

using DFCommonLib.Config;

namespace BotWebServer.Model
{
    public class BotConfigurationSettings : ConfigurationSettings
    {
        public new BotCustomerConfiguration CustomerSettings { get; set; }
    }
}
