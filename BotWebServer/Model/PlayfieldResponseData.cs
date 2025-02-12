using System;
using System.Collections.Generic;

namespace BotWebServer.Model
{
    public class PlayfieldResponseData
    {
        public static uint Ok = 0;
        public static uint ErrorNotLoggedIn = 1;
        public static uint NotOwnerOfPlayfield = 2;
        public static uint UnknownError = 3;

        public string uuid { get; set; }
        public uint errorId{ get; set; }
        public string message { get; set; }

        public PlayfieldResponseData()
        {
        }

        public PlayfieldResponseData(string uuid, uint errorId, string message)
        {
            this.uuid = uuid;
            this.errorId = errorId;
            this.message = message;
        }
    }
}
