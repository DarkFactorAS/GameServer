using System;
using System.Collections.Generic;

namespace BotWebServer.Model
{
    public class PlayfieldData
    {
        public uint id { get; set; }
        public string uniqueId { get; set; }
        public uint revisionId { get; set; }
        public string owner { get; set; }
        public string name { get; set; }
        public string description { get; set; }
        public uint playfieldFlags { get; set; }
        public uint numPlayers { get; set; }
        public uint numGoals { get; set; }
        public uint boardSizeX { get; set; }
        public uint boardSizeY { get; set; }
        public uint version { get; set; }
        public string data { get; set; }

        public PlayfieldData()
        {
        }
    }


}
