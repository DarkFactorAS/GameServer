using System;
using System.Collections.Generic;

namespace BotWebServer.Model
{
    public class PlayfieldList
    {
       public IList<PlayfieldData> list { get; set; }
       public PlayfieldList()
       {
           list = new List<PlayfieldData>();
       }
    }
}