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
        [Route("GetPlayfieldList")]
        public PlayfieldList GetPlayfieldList()
        {
            return _repository.GetPlayfieldList();
        }

        [HttpGet]
        [Route("GetPlayfield")]
        public PlayfieldData GetPlayfield(uint playfieldId)
        {
            return _repository.GetPlayfield(playfieldId);
        }

        [HttpPut]
        [Route("SavePlayfield")]
        public PlayfieldResponseData SavePlayfield( PlayfieldData playfieldData )
        {
            if ( playfieldData != null )
            {
                _logger.LogDebug("Save playfield ...", playfieldData.id);
                return _repository.SavePlayfield(playfieldData);
            }
            else
            {
                _logger.LogDebug("Save playfield failed");
                return new PlayfieldResponseData( playfieldData.id, "Failed to save playfield");
            }
        }

        [HttpPut]
        [Route("DeletePlayfield")]
        public PlayfieldResponseData DeletePlayfield( uint playfieldId )
        {
            _logger.LogDebug("Delete playfield failed");
            return new PlayfieldResponseData( playfieldId, "Failed to delete playfield");
        }
    }
}
