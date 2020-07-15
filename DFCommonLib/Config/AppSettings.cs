using System;
using System.Collections.Generic;
using System.Text;

namespace DFCommonLib.Config
{
    public class AppSettings
    {
        public string JobName { get; set; }
        public string SecretKey { get; set; }
        public int TimerInterval { get; set; }
    }
}
