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
        PlayfieldList GetPlayfieldList(string ownerName);
        PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData, string ownerName);
        PlayfieldResponseData DeletePlayfield(uint playfieldId, string ownerName);
    }

    public class PlayfieldRepository : IPlayfieldRepository
    {
        private IDbConnectionFactory _connection;
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
            playfieldData.uniqueId = reader["uniqueId"].ToString(); 
            playfieldData.revisionId = Convert.ToUInt32(reader["revisionid"]);
            playfieldData.owner = reader["ownerid"].ToString(); 
            playfieldData.name = reader["name"].ToString();
            playfieldData.description = reader["description"].ToString();
            playfieldData.playfieldFlags = Convert.ToUInt32(reader["playfieldFlags"]);
            playfieldData.numPlayers = Convert.ToUInt32(reader["numPlayers"]);
            playfieldData.numGoals = Convert.ToUInt32(reader["numGoals"]);
            playfieldData.boardSizeX = Convert.ToUInt32(reader["boardSizeX"]);
            playfieldData.boardSizeY = Convert.ToUInt32(reader["boardSizeY"]);
            playfieldData.version = Convert.ToUInt32(reader["version"]);

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

        public PlayfieldList GetPlayfieldList(string ownerName)
        {
            // Get specific playfield
            PlayfieldList playfieldList = new PlayfieldList();

            _logger.LogInfo("GetPlayfieldList");

            var sql = @"SELECT * 
                FROM playfield 
                where ( ownerid = @ownerid or playfieldFlags = 2 )
                order by updated desc limit 100";
            using (var cmd = _connection.CreateCommand(sql))
            {
                cmd.AddParameter("@ownerid", ownerName);

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

        public PlayfieldResponseData SavePlayfield(PlayfieldData playfieldData, string ownerName)
        {
            byte[] data = Convert.FromBase64String(playfieldData.data);

            var playfield = GetPlayfield(playfieldData.id);
            if ( playfield != null )
            {
                int numRows = 0;
                var sql = @"UPDATE playfield set 
                    uniqueId = @uniqueId,
                    revisionid = @revisionid,
                    name = @name, 
                    description = @description,
                    numPlayers = @numPlayers,
                    numGoals = @numGoals,
                    version = @version,
                    updated = now(),
                    data=@data 
                    where id = @id
                    and ownerid = @ownerid";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@uniqueId", playfieldData.uniqueId);
                    cmd.AddParameter("@revisionid", playfieldData.revisionId);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@description", playfieldData.description);
                    cmd.AddParameter("@numPlayers", playfieldData.numPlayers);
                    cmd.AddParameter("@numGoals", playfieldData.numGoals);
                    cmd.AddParameter("@version", playfieldData.version);
                    cmd.AddClobParameter("@data", data);
                    cmd.AddParameter("@ownerid", ownerName);
                    numRows = cmd.ExecuteNonQuery();
                }

                if ( numRows == 0 )
                {
                    return new PlayfieldResponseData(playfieldData.id, PlayfieldResponseData.NotOwnerOfPlayfield, "Failed to save playfield ( No access to playfield )");
                }
                return new PlayfieldResponseData(playfieldData.id, PlayfieldResponseData.Ok, "Playfield saved successfully");
            }
            else
            {
                var sql = @"INSERT INTO playfield(id, uniqueId, revisionid, updated, ownerid, name, description,
                    playfieldFlags, numPlayers, numGoals, boardSizeX, boardSizeY, version,data) 
                    VALUES(@id, @uniqueId, @revisionid, now(), @ownerid, @name, @description, 
                    @playfieldFlags, @numPlayers, @numGoals, @boardSizeX, @boardSizeY, @version, @data)";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@id", playfieldData.id);
                    cmd.AddParameter("@uniqueId", playfieldData.uniqueId);
                    cmd.AddParameter("@revisionid", playfieldData.revisionId);
                    cmd.AddParameter("@ownerid", ownerName);
                    cmd.AddParameter("@name", playfieldData.name);
                    cmd.AddParameter("@description", playfieldData.description);
                    cmd.AddParameter("@playfieldFlags", playfieldData.playfieldFlags);
                    cmd.AddParameter("@numPlayers", playfieldData.numPlayers);
                    cmd.AddParameter("@numGoals", playfieldData.numGoals);
                    cmd.AddParameter("@boardSizeX", playfieldData.boardSizeX);
                    cmd.AddParameter("@boardSizeY", playfieldData.boardSizeY);
                    cmd.AddParameter("@version", playfieldData.version);
                    cmd.AddClobParameter("@data", data);
                    cmd.ExecuteNonQuery();
                }

                var playfieldId = GetId();
                return new PlayfieldResponseData(playfieldId, PlayfieldResponseData.Ok, "Playfield saved successfully");
            }
        }

        public PlayfieldResponseData DeletePlayfield(uint playfieldId, string ownerName)
        {
            try
            {
                var sql = @"DELETE FROM playfield where id = @playfieldId and ownerid=@ownerid";
                using (var cmd = _connection.CreateCommand(sql))
                {
                    cmd.AddParameter("@playfieldId", playfieldId);
                    cmd.AddParameter("@ownerid", ownerName);
                    var rows = cmd.ExecuteNonQuery();
                    if ( rows > 0)
                    {
                        return new PlayfieldResponseData(playfieldId, PlayfieldResponseData.Ok, "Playfield deleted.");
                    }
                }
            }
            catch( Exception ex )
            {
                _logger.LogError(ex.ToString());
            }
            return new PlayfieldResponseData(0, PlayfieldResponseData.UnknownError, "Failed to delete playfield.");
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