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

        public uint playfieldId { get; set; }
        public uint errorId{ get; set; }
        public string message { get; set; }

        public PlayfieldResponseData()
        {
        }

        public PlayfieldResponseData(uint playfieldId, uint errorId, string message)
        {
            this.playfieldId = playfieldId;
            this.errorId = errorId;
            this.message = message;
        }
    }
}
