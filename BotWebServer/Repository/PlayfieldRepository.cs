using System;
using BotWebServer.Model;
using DFCommonLib.DataAccess;
using DFCommonLib.Config;
using System.Linq;

namespace BotWebServer.Repository
{
    public interface IPlayfieldRepository
    {
        PlayfieldData GetPlayfield(uint playfieldId);
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

        public PlayfieldData GetPlayfield(uint playfieldId)
        {
            if ( playfieldId == 0 )
            {
                return null;
            }

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

                        playfieldData.id = Convert.ToUInt32(reader["id"]);
                        playfieldData.name = reader["name"].ToString();
                        return playfieldData;
                    }
                }
            }
            return null;
        }

        public void SavePlayfield(PlayfieldData playfieldData)
        {
            string owner = "Cha00z";

            var playfield = GetPlayfield(playfieldData.id);
            if ( playfield != null )
            {
                var sql = @"UPDATE playfield set 
                    name = @name, 
                    description = @description,
                    numPlayers = @numPlayers,
                    numGoals = @numGoals,
                    updated = now(),
                    data=@data 
                    where id = @id";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@description", playfieldData.description);
                    cmd.AddParameter("@numPlayers", playfieldData.numPlayers);
                    cmd.AddParameter("@numGoals", playfieldData.numGoals);
                    cmd.AddParameter("@data", playfieldData.data);
                    cmd.ExecuteNonQuery();
                }
            }
            else
            {
                var sql = @"INSERT INTO playfield(id, updated, ownerid, name, description,
                    playfieldFlags, numPlayers, numGoals, boardSizeX, boardSizeY, data) 
                    VALUES(@id,now(), @ownerid, @name, @description, 
                    @playfieldFlags, @numPlayers, @numGoals, @boardSizeX, @boardSizeY,@data)";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@ownerid", owner);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@description", playfieldData.description);
                    cmd.AddParameter("@playfieldFlags", playfieldData.playfieldFlags);
                    cmd.AddParameter("@numPlayers", playfieldData.numPlayers);
                    cmd.AddParameter("@numGoals", playfieldData.numGoals);
                    cmd.AddParameter("@boardSizeX", playfieldData.boardSizeX);
                    cmd.AddParameter("@boardSizeY", playfieldData.boardSizeY);
                    cmd.AddParameter("@data", playfieldData.data);
                    cmd.ExecuteNonQuery();
                }
            }
        }
    }
}