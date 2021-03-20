using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using BotWebServer.Model;
using BotWebServer.Repository;
using BotWebServer.Provider;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class PlayfieldController : ControllerBase
    {
        IPlayfieldProvider _provider;

        public PlayfieldController( IPlayfieldProvider provider )
        {
            _provider = provider;
        }

        [HttpGet]
        [Route("GetPlayfieldList")]
        public PlayfieldList GetPlayfieldList()
        {
            return _provider.GetPlayfieldList();
        }

        [HttpGet]
        [Route("GetPlayfield")]
        public PlayfieldData GetPlayfield(uint playfieldId)
        {
            return _provider.GetPlayfield(playfieldId);
        }

        [HttpPut]
        [Route("SavePlayfield")]
        public PlayfieldResponseData SavePlayfield( PlayfieldData playfieldData )
        {
            return _provider.SavePlayfield(playfieldData);
        }

        [HttpPut]
        [Route("DeletePlayfield")]
        public PlayfieldResponseData DeletePlayfield( uint playfieldId )
        {
            return _provider.DeletePlayfield(playfieldId);
        }
    }
}
