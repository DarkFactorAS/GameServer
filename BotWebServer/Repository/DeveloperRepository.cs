namespace BotWebServer.Repository
{
    public interface IDeveloperRepository
    {
        int GetDeveloperFlags(string username);
    }

    public class DeveloperRepository : IDeveloperRepository
    {
        public int GetDeveloperFlags(string username)
        {
            return 1;
        }
    }
}