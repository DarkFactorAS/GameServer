using System;
using System.Collections.Generic;
using BotWebServer.Model;
using DFCommonLib.DataAccess;
using DFCommonLib.Logger;

namespace BotWebServer.Repository
{
    public interface IPlayfieldRepository
    {
        PlayfieldData GetPlayfield(uint playfieldId);
        PlayfieldList GetPlayfieldList();
        PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData);
    }

    public class PlayfieldRepository : IPlayfieldRepository
    {
        private IDbConnectionFactory _connection;

        private string owner = "Cha00z";

        private readonly IDFLogger<PlayfieldRepository> _logger;

        public PlayfieldRepository(
            IDbConnectionFactory connection,
            IDFLogger<PlayfieldRepository> logger
            )
        {
            _connection = connection;
            _logger = logger;
        }

        public PlayfieldData GetPlayfield(uint playfieldId)
        {
            if ( playfieldId == 0 )
            {
                return null;
            }

            // Get specific playfield
            var sql = @"SELECT * FROM playfield WHERE id = @id";
            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter("@id", playfieldId);
                using (var reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        PlayfieldData playfieldData = ReadRow(reader);
                        return playfieldData;
                    }
                }
            }
            return null;
        }

        private PlayfieldData ReadRow(System.Data.IDataReader reader )
        {
            PlayfieldData playfieldData = new PlayfieldData();

            playfieldData.id = Convert.ToUInt32(reader["id"]);
            playfieldData.owner = reader["ownerid"].ToString(); 
            playfieldData.name = reader["name"].ToString();
            playfieldData.description = reader["description"].ToString();
            playfieldData.playfieldFlags = Convert.ToUInt32(reader["playfieldFlags"]);
            playfieldData.numPlayers = Convert.ToUInt32(reader["numPlayers"]);
            playfieldData.numGoals = Convert.ToUInt32(reader["numGoals"]);
            playfieldData.boardSizeX = Convert.ToUInt32(reader["boardSizeX"]);
            playfieldData.boardSizeY = Convert.ToUInt32(reader["boardSizeY"]);

            // Read data
            byte[] buffer = new byte[10000];
            int index = reader.GetOrdinal("data");
            long numBytes = reader.GetBytes(index, 0, buffer, 0, 10000);

            if ( numBytes > 0 )
            {
                playfieldData.data = Convert.ToBase64String( buffer, 0, (int)numBytes );
            }

            return playfieldData;
        }

        public PlayfieldList GetPlayfieldList()
        {
            // Get specific playfield
            PlayfieldList playfieldList = new PlayfieldList();

            _logger.LogInfo("GetPlayfieldList");

            var sql = @"SELECT id,ownerid,name,description,playfieldFlags,numPlayers,numGoals,boardSizeX, boardSizeY, data FROM playfield order by updated desc limit 10";
            using (var cmd = _connection.CreateCommand(sql))
            {
                using (var reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        PlayfieldData playfieldData = ReadRow(reader);
                        playfieldList.list.Add(playfieldData);
                    }
                }
            }

            _logger.LogInfo( string.Format("GetPlayfieldList : {0}", playfieldList.list.Count));

            return playfieldList;
        }

        public PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData)
        {
            byte[] data = Convert.FromBase64String(playfieldData.data);

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
                    cmd.AddClobParameter("@data", data);
                    cmd.ExecuteNonQuery();
                }

                return new PlayfieldResponseData(playfieldData.id, "Playfield saved successfully");
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
                    cmd.AddClobParameter("@data", data);
                    cmd.ExecuteNonQuery();
                }

                var playfieldId = GetId();
                return new PlayfieldResponseData(playfieldId, "Playfield saved successfully");
            }
        }

        private uint GetId()
        {
            var sql = @"SELECT LAST_INSERT_ID() as id";
            using (var cmd = _connection.CreateCommand(sql))
            {
                using (var reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        uint id = Convert.ToUInt32(reader["id"]);
                        return id;
                    }
                }
            }
            return 0;
        }
     }
}