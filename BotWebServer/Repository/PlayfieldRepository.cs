using System;
using BotWebServer.Model;
using DFCommonLib.DataAccess;
using DFCommonLib.Config;
using System.Linq;

namespace BotWebServer.Repository
{
    public interface IPlayfieldRepository
    {
        PlayfieldData GetPlayfield(int playfieldId);
        void SavePlayfield(PlayfieldData playfieldData);
    }

    public class PlayfieldRepository : IPlayfieldRepository
    {
        private IDbConnectionFactory _connection;

        public PlayfieldRepository(IConfigurationHelper configuration)
        {
            Customer customer = configuration.ConfigurationSettings.CustomerSettings.Customers.FirstOrDefault();
            _connection = new LocalMysqlConnectionFactory(customer);
        }

        public PlayfieldData GetPlayfield(int playfieldId)
        {
            // Get specific playfield
            var sql = @"SELECT id,name,data FROM playfield WHERE id = @id";
            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter("@id", playfieldId);
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

        public void SavePlayfield(PlayfieldData playfieldData)
        {
            var playfield = GetPlayfield(playfieldData.id);
            if ( playfield != null )
            {
                var sql = @"UPDATE playfield set name = @name, data=@data where id = @id";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@data", playfieldData.data);
                    cmd.ExecuteNonQuery();
                }
            }
            else
            {
                var sql = @"INSERT INTO playfield(id,name,data) VALUES(@id,@name,@data)";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@data", playfieldData.data);
                    cmd.ExecuteNonQuery();
                }
            }
        }
    }
}