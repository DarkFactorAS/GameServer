using System.Collections.Generic;

namespace BotWebServer.Model
{
    public class PlayerStatisticsData
    {
        public int totalWins { get; set; }
        public int totalStars { get; set; }
        public int totalGamesPlayed { get; set; }
        public IList<PlayerPlayfieldStatisticsData> playfieldStatistics { get; set; }

        public PlayerStatisticsData()
        {
            playfieldStatistics = new List<PlayerPlayfieldStatisticsData>();
        }
    }
}
