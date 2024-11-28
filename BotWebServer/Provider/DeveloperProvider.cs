
using BotWebServer.Model;
using BotWebServer.Repository;

namespace BotWebServer.Provider
{
    public interface IDeveloperProvider
    {
        int EnableDeveloper(bool enable);
    }

    public class DeveloperProvider : IDeveloperProvider
    {
        IDeveloperRepository _repository;
       IBotSessionProvider _session;
 
        public DeveloperProvider(IDeveloperRepository repository, IBotSessionProvider session )
        {
            _repository = repository;
            _session = session;
        }

        public int EnableDeveloper(bool enable)
        {
            if ( !_session.IsLoggedIn() )
            {
                return 0;
            }

            if (!enable)
            {
                _session.SetDeveloperFlags(0);
                return 0;
            }

            var username = _session.GetNickname();
            return _repository.GetDeveloperFlags(username);
        }
    }
}
