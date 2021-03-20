using Microsoft.AspNetCore.Http;

namespace BotWebServer.Provider
{
    public interface IBotSessionProvider : IUserSessionProvider
    {
        void SetUser(string nickname,string token);
        string GetNickname();
        string GetToken();
        bool IsLoggedIn();
    }

    public class BotSessionProvider : UserSessionProvider, IBotSessionProvider
    {
        public static readonly string SessionNicknameKey = "Nickname";
        public static readonly string SessionTokenKey = "Token";

        public BotSessionProvider( IHttpContextAccessor httpContext ) : base( "BotServer", httpContext )
        {
        }

        override public void RemoveSession()
        {
            RemoveConfig(SessionNicknameKey);
            RemoveConfig(SessionTokenKey);
        }

        public void SetUser(string nickname, string token)
        {
            RemoveSession();
            SetConfigString(SessionNicknameKey, nickname);
            SetConfigString(SessionTokenKey, token);
        }

        public string GetNickname()
        {
            return GetConfigString(SessionNicknameKey);
        }

        public string GetToken()
        {
            return GetConfigString(SessionTokenKey);
        }

        public bool IsLoggedIn()
        {
            var token = GetConfigString(SessionTokenKey);
            if ( token != null )
            {
                return true;
            }
            return false;
        }

    }
}