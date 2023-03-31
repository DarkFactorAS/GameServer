using DFCommonLib.DataAccess;
using DFCommonLib.Logger;

namespace BotWebServer.Repository
{
    public interface IStartupRepository
    {
        bool RunPatcher();
    }

    public class StartupRepository : IStartupRepository
    {
        private static string PATCHER = "BotServer";

        private IDBPatcher _dbPatcher;

        public StartupRepository(IDBPatcher dbPatcher)
        {
            _dbPatcher = dbPatcher;
        }

        public bool RunPatcher()
        {
            _dbPatcher.Init();

            // Create log table
            _dbPatcher.Patch(PATCHER,1, "CREATE TABLE `logtable` ( `id` int(11) NOT NULL AUTO_INCREMENT, `created` datetime NOT NULL, `loglevel` int(11) NOT NULL, `groupname` varchar(100) NOT NULL DEFAULT '', `message` varchar(1024) NOT NULL DEFAULT '', PRIMARY KEY (`id`))");

            // Playfield table
            _dbPatcher.Patch(PATCHER,2, "CREATE TABLE `playfield` ("
            + " `id` int(11) NOT NULL AUTO_INCREMENT, " 
            + " `revisionid` int(11) NOT NULL DEFAULT 1,"
            + " `updated` datetime NOT NULL,"
            + " `ownerid` varchar(30) NOT NULL DEFAULT '', "
            + " `name` varchar(30) NOT NULL DEFAULT '', "
            + " `description` varchar(128) NOT NULL DEFAULT '', "
            + " `playfieldFlags` int(11) NOT NULL DEFAULT 0,"
            + " `numPlayers` int(11) NOT NULL DEFAULT 0,"
            + " `numGoals` int(11) NOT NULL DEFAULT 0,"
            + " `boardSizeX` int(11) NOT NULL DEFAULT 0,"
            + " `boardSizeY` int(11) NOT NULL DEFAULT 0,"
            + " `version` int(11) NOT NULL DEFAULT 1,"
            + " `data` mediumblob DEFAULT NULL, "
            + " PRIMARY KEY (`id`)"
            + ")"
            );

            return _dbPatcher.Successful();
        }
    }
}