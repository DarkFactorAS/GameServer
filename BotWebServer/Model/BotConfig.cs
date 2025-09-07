using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

using DFCommonLib.Config;

namespace BotWebServer.Model
{
    public class BotConfig : AppSettings
    {
        public string AccountServer { get; set; }
    }
}
