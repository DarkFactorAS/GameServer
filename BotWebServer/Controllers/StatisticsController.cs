using Microsoft.AspNetCore.Mvc;
using BotWebServer.Model;
using BotWebServer.Provider;
using DFCommonLib.HttpApi.OAuth2;
using Microsoft.AspNetCore.Authorization;

namespace BotWebServer.Controllers
{
    [Authorize(AuthenticationSchemes = OAuth2Static.AuthenticationScheme)]
    public class StatisticsController : DFRestOAuth2ServerController
    {
        IStatisticsProvider _provider;

        public StatisticsController(IStatisticsProvider provider)
        {
            _provider = provider;
        }

        [HttpGet]
        [Route("GetStatistics")]
        public PlayerStatisticsData GetStatistics()
        {
            return _provider.GetStatistics();
        }

        [HttpPut]
        [Route("UpdateStatistics")]
        public StatisticsResponseData UpdateStatistics(PlayerStatisticsData statistics)
        {
            return _provider.UpdateStatistics(statistics);
        }
    }
}
