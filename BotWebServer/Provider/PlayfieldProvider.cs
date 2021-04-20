using Microsoft.Extensions.Logging;

using BotWebServer.Model;
using BotWebServer.Repository;
using DFCommonLib.Logger;

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
        IDFLogger<PlayfieldProvider> _logger;

        public PlayfieldProvider( IDFLogger<PlayfieldProvider> logger,
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
            if ( !_session.IsLoggedIn() )
            {
                _logger.LogDebug("Save playfield failed : Not logged in");
                return new PlayfieldList();
            }

            var nickname = _session.GetNickname();
            if ( string.IsNullOrEmpty(nickname) )
            {
                _logger.LogDebug("Save playfield failed : unknown nickname");
                return new PlayfieldList();
            }

            return _repository.GetPlayfieldList(nickname);
        }

        public PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData)
        {
            if ( playfieldData == null )
            {
                _logger.LogDebug("Save playfield failed : No data");
                return new PlayfieldResponseData( playfieldData.id, PlayfieldResponseData.ErrorNotLoggedIn, "Failed to save playfield");
            }

            if ( !_session.IsLoggedIn() )
            {
                _logger.LogDebug("Save playfield failed : Not logged in");
                return new PlayfieldResponseData(playfieldData.id, PlayfieldResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            var nickname = _session.GetNickname();
            if ( string.IsNullOrEmpty(nickname) )
            {
                _logger.LogDebug("Save playfield failed : unknown nickname");
                return new PlayfieldResponseData(playfieldData.id, PlayfieldResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            return _repository.SavePlayfield(playfieldData, nickname);
        }

        public PlayfieldResponseData DeletePlayfield( uint playfieldId )
        {
             if ( !_session.IsLoggedIn() )
            {
                _logger.LogDebug("Delete playfield failed : Not logged in");
                return new PlayfieldResponseData(playfieldId, PlayfieldResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            var nickname = _session.GetNickname();
            if ( string.IsNullOrEmpty(nickname) )
            {
                _logger.LogDebug("Delete playfield failed : unknown nickname");
                return new PlayfieldResponseData(playfieldId, PlayfieldResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            return _repository.DeletePlayfield(playfieldId, nickname);
        }
    }
}