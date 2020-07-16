using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using BotWebServer.Model;
using BotWebServer.Repository;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class PlayfieldController : ControllerBase
    {
        ILogger<PlayfieldController> _logger;
        IPlayfieldRepository _repository;

        public PlayfieldController(ILogger<PlayfieldController> logger, IPlayfieldRepository repository)
        {
            _logger = logger;
            _repository = repository;
        }

        [HttpGet]
        public PlayfieldData GetPlayfield(int playfieldId)
        {
            return _repository.GetPlayfield(playfieldId);
        }

        [HttpPut]
        public void SavePlayfield( PlayfieldData playfieldData )
        {
            if ( playfieldData != null )
            {
                _logger.LogDebug("Save playfield ...", playfieldData.id);
                _repository.SavePlayfield(playfieldData);
            }
            else
            {
                _logger.LogDebug("Save playfield ...", playfieldData.id);
            }
        }
    }
}
