using System;
using BotWebServer.Model;
using DFCommonLib.DataAccess;
using DFCommonLib.Logger;

namespace BotWebServer.Repository
{
    public interface IStatisticsRepository
    {
        PlayerStatisticsData GetPlayerStatistics(string playerId);
        StatisticsResponseData UpdatePlayerStatistics(string playerId, PlayerStatisticsData statistics);
    }

    public class StatisticsRepository : IStatisticsRepository
    {
        private IDbConnectionFactory _connection;
        private readonly IDFLogger<StatisticsRepository> _logger;

        public StatisticsRepository(
            IDbConnectionFactory connection,
            IDFLogger<StatisticsRepository> logger)
        {
            _connection = connection;
            _logger = logger;
        }

        public PlayerStatisticsData GetPlayerStatistics(string playerId)
        {
            var statistics = new PlayerStatisticsData();

            var sql = @"SELECT totalWins, totalStars, totalGamesPlayed FROM player_statistics WHERE playerid = @playerid";
            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter("@playerid", playerId);
                using (var reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        statistics.totalWins = Convert.ToInt32(reader["totalWins"]);
                        statistics.totalStars = Convert.ToInt32(reader["totalStars"]);
                        statistics.totalGamesPlayed = Convert.ToInt32(reader["totalGamesPlayed"]);
                    }
                }
            }

            var pfSql = @"SELECT playfieldUUID, gamesPlayed, wins FROM player_playfield_statistics WHERE playerid = @playerid";
            using (var cmd = _connection.CreateCommand(pfSql))
            {
                cmd.AddParameter("@playerid", playerId);
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        statistics.playfieldStatistics.Add(new PlayerPlayfieldStatisticsData
                        {
                            playfieldUUID = reader["playfieldUUID"].ToString(),
                            gamesPlayed = Convert.ToInt32(reader["gamesPlayed"]),
                            wins = Convert.ToInt32(reader["wins"])
                        });
                    }
                }
            }

            return statistics;
        }

        public StatisticsResponseData UpdatePlayerStatistics(string playerId, PlayerStatisticsData statistics)
        {
            try
            {
                var sql = @"INSERT INTO player_statistics (playerid, totalWins, totalStars, totalGamesPlayed)
                    VALUES (@playerid, @totalWins, @totalStars, @totalGamesPlayed)
                    ON DUPLICATE KEY UPDATE totalWins = @totalWins, totalStars = @totalStars, totalGamesPlayed = @totalGamesPlayed";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@playerid", playerId);
                    cmd.AddParameter("@totalWins", statistics.totalWins);
                    cmd.AddParameter("@totalStars", statistics.totalStars);
                    cmd.AddParameter("@totalGamesPlayed", statistics.totalGamesPlayed);
                    cmd.ExecuteNonQuery();
                }

                if (statistics.playfieldStatistics != null)
                {
                    foreach (var pf in statistics.playfieldStatistics)
                    {
                        UpdatePlayfieldStatistics(playerId, pf);
                    }
                }

                return new StatisticsResponseData(StatisticsResponseData.Ok, "Statistics updated successfully");
            }
            catch (Exception ex)
            {
                _logger.LogError(ex.ToString());
                return new StatisticsResponseData(StatisticsResponseData.UnknownError, "Failed to update statistics");
            }
        }

        private void UpdatePlayfieldStatistics(string playerId, PlayerPlayfieldStatisticsData pfData)
        {
            var sql = @"INSERT INTO player_playfield_statistics (playerid, playfieldUUID, gamesPlayed, wins)
                VALUES (@playerid, @playfieldUUID, @gamesPlayed, @wins)
                ON DUPLICATE KEY UPDATE gamesPlayed = @gamesPlayed, wins = @wins";
            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter("@playerid", playerId);
                cmd.AddParameter("@playfieldUUID", pfData.playfieldUUID);
                cmd.AddParameter("@gamesPlayed", pfData.gamesPlayed);
                cmd.AddParameter("@wins", pfData.wins);
                cmd.ExecuteNonQuery();
            }
        }
    }
}
