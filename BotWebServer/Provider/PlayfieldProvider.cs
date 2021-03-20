using Microsoft.Extensions.Logging;

using BotWebServer.Model;
using BotWebServer.Repository;

namespace BotWebServer.Provider
{
    public interface IPlayfieldProvider
    {
        PlayfieldData GetPlayfield(uint playfieldId);
        PlayfieldList GetPlayfieldList();
        PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData);
        PlayfieldResponseData DeletePlayfield( uint playfieldId );
    }

    public class PlayfieldProvider : IPlayfieldProvider
    {
        IPlayfieldRepository _repository;
        IBotSessionProvider _session;
        ILogger<PlayfieldProvider> _logger;

        public PlayfieldProvider( ILogger<PlayfieldProvider> logger,
            IPlayfieldRepository repository, 
            IBotSessionProvider session )
        {
            _logger = logger;
            _repository = repository;
            _session = session;
        }

        public PlayfieldData GetPlayfield(uint playfieldId)
        {
            return _repository.GetPlayfield(playfieldId);
        }

        public PlayfieldList GetPlayfieldList()
        {
            return _repository.GetPlayfieldList();
        }

        public PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData)
        {
            if ( playfieldData == null )
            {
                _logger.LogDebug("Save playfield failed : No data");
                return new PlayfieldResponseData( playfieldData.id, "Failed to save playfield");
            }

            if ( !_session.IsLoggedIn() )
            {
                _logger.LogDebug("Save playfield failed : Not logged in");
                return new PlayfieldResponseData(playfieldData.id, "Not logged in");
            }

            var nickname = _session.GetNickname();
            if ( string.IsNullOrEmpty(nickname) )
            {
                _logger.LogDebug("Save playfield failed : unknown nickname");
                return new PlayfieldResponseData(playfieldData.id, "Not logged in");
            }

            return _repository.SavePlayfield(playfieldData, nickname);
        }

        public PlayfieldResponseData DeletePlayfield( uint playfieldId )
        {
            _logger.LogDebug("Delete playfield failed");
            return new PlayfieldResponseData( playfieldId, "Failed to delete playfield");
        }
    }
}