using DFCommonLib.DataAccess;
using DFCommonLib.Logger;

namespace BotWebServer.Repository
{
    public class BotDatabasePatcher : StartupDatabasePatcher
    {
        private static string PATCHER = "BotServer";

        public BotDatabasePatcher(IDBPatcher dbPatcher) : base(dbPatcher)
        {
        }

        public override bool RunPatcher()
        {
            base.RunPatcher();
            
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

            // Add key
            _dbPatcher.Patch(PATCHER,3, "ALTER TABLE `playfield` ADD `uniqueId` varchar(20) NOT NULL AFTER `id`;");

            return _dbPatcher.Successful();
        }
    }
}