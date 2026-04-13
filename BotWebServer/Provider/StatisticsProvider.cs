using BotWebServer.Model;
using BotWebServer.Repository;
using DFCommonLib.Logger;

namespace BotWebServer.Provider
{
    public interface IStatisticsProvider
    {
        PlayerStatisticsData GetStatistics();
        StatisticsResponseData UpdateStatistics(PlayerStatisticsData statistics);
    }

    public class StatisticsProvider : IStatisticsProvider
    {
        IStatisticsRepository _repository;
        IBotSessionProvider _session;
        IDFLogger<StatisticsProvider> _logger;

        public StatisticsProvider(
            IDFLogger<StatisticsProvider> logger,
            IStatisticsRepository repository,
            IBotSessionProvider session)
        {
            _logger = logger;
            _repository = repository;
            _session = session;
        }

        public PlayerStatisticsData GetStatistics()
        {
            if (!_session.IsLoggedIn())
            {
                _logger.LogDebug("GetStatistics failed : Not logged in");
                return new PlayerStatisticsData();
            }

            var playerId = _session.GetAccountId();
            if (string.IsNullOrEmpty(playerId))
            {
                _logger.LogDebug("GetStatistics failed : unknown player id");
                return new PlayerStatisticsData();
            }

            return _repository.GetPlayerStatistics(playerId);
        }

        public StatisticsResponseData UpdateStatistics(PlayerStatisticsData statistics)
        {
            if (statistics == null)
            {
                _logger.LogDebug("UpdateStatistics failed : No data");
                return new StatisticsResponseData(StatisticsResponseData.UnknownError, "No statistics data provided");
            }

            if (!_session.IsLoggedIn())
            {
                _logger.LogDebug("UpdateStatistics failed : Not logged in");
                return new StatisticsResponseData(StatisticsResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            var playerId = _session.GetAccountId();
            if (string.IsNullOrEmpty(playerId))
            {
                _logger.LogDebug("UpdateStatistics failed : unknown player id");
                return new StatisticsResponseData(StatisticsResponseData.ErrorNotLoggedIn, "Not logged in");
            }

            return _repository.UpdatePlayerStatistics(playerId, statistics);
        }
    }
}
