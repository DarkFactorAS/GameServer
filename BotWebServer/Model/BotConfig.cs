using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

using DFCommonLib.Config;

namespace BotWebServer.Model
{
    public class ApiSettings
    {
        public string Endpoint { get; set; }
        public string ClientId { get; set; }
        public string ClientSecret { get; set; }
        public string Scope { get; set; }
    }

    public class BotConfig : AppSettings
    {
        public ApiSettings AccountServer { get; set; }
    }
}
