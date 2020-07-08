using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using BotWebServer.Model;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class PlayfieldController : ControllerBase
    {
        ILogger<PlayfieldController> _logger;

        public PlayfieldController(ILogger<PlayfieldController> logger)
        {
            _logger = logger;
        }

        [HttpGet]
        public PlayfieldData GetPlayfield(int playfieldId)
        {
            return null;
        }

        [HttpPost]
        public void SavePlayfield( PlayfieldData playfieldData )
        {
            if ( playfieldData != null )
            {
                _logger.LogDebug("Save playfield ...", playfieldData.id);
            }
            else
            {
                _logger.LogDebug("Save playfield ...", playfieldData.id);
            }
        }
    }
}
