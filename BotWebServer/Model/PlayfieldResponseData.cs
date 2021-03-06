using System;
using System.Collections.Generic;

namespace BotWebServer.Model
{
    public class PlayfieldResponseData
    {
        public uint id { get; set; }
        public string message { get; set; }

        public PlayfieldResponseData()
        {
        }

        public PlayfieldResponseData(uint id, string message)
        {
            this.id = id;
            this.message = message;
        }
    }
}
