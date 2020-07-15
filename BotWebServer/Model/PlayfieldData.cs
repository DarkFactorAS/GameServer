using System;
namespace BotWebServer.Model
{
    public class PlayfieldData
    {
        public int id { get; set; }
        public string name { get; set; }
        public byte[] data { get; set; }

        public PlayfieldData()
        {
        }
    }
}
