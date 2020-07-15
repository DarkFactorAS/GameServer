using System;
using BotWebServer.Model;
using DFCommonLib.DataAccess;
using DFCommonLib.Config;
using System.Linq;

namespace BotWebServer.Repository
{
    public class PlayfieldRepository
    {
        private IDbConnectionFactory _connection;

        public PlayfieldRepository(IConfigurationHelper configuration)
        {
            Customer customer = configuration.ConfigurationSettings.CustomerSettings.Customers.FirstOrDefault();
            _connection = new LocalMysqlConnectionFactory(customer);
        }

        public PlayfieldData GetPlayfield(int playfieldId)
        {
            // Get new notifications
            var sql = @"SELECT id,name,data FROM playfield                    
                    WHERE id = :id
                    ORDER BY CREATEDTIME DESC";

            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter(":id", playfieldId);
                using (var reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        PlayfieldData playfieldData = new PlayfieldData();

                        playfieldData.id = Convert.ToInt32(reader["id"]);
                        playfieldData.name = reader["name"].ToString();

                        return playfieldData;
                    }
                }
            }
            return null;
        }
    }
}