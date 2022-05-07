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
            _dbPatcher.Patch(PATCHER,3, "ALTER TABLE playfield ADD COLUMN version int(11) NOT NULL DEFAULT 7 AFTER boardSizeY");
            return _dbPatcher.Successful();
        }

    }
}