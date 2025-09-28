using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using BotWebServer.Model;
using BotWebServer.Repository;
using BotWebServer.Provider;
using DFCommonLib.HttpApi.OAuth2;
using Microsoft.AspNetCore.Authorization;

namespace BotWebServer.Controllers
{
    [Authorize(AuthenticationSchemes = OAuth2Static.AuthenticationScheme)]
    public class PlayfieldController : DFRestOAuth2ServerController
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
        public PlayfieldData GetPlayfield(string uuid)
        {
            return _provider.GetPlayfield(uuid);
        }

        [HttpPut]
        [Route("SavePlayfield")]
        public PlayfieldResponseData SavePlayfield( PlayfieldData playfieldData )
        {
            return _provider.SavePlayfield(playfieldData);
        }

        [HttpPut]
        [Route("DeletePlayfield")]
        public PlayfieldResponseData DeletePlayfield([FromBody] string uuid )
        {
            return _provider.DeletePlayfield(uuid);
        }
    }
}
