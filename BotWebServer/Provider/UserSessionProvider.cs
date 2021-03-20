using Microsoft.AspNetCore.Http;

namespace BotWebServer.Provider
{
    public interface IUserSessionProvider
    {
        void RemoveSession();
    }

    public class UserSessionProvider : IUserSessionProvider
    {

        private readonly IHttpContextAccessor _httpContextAccessor;
        private string _sessionName;

        public UserSessionProvider( string sessionName, IHttpContextAccessor httpContext )
        {
            _sessionName = sessionName;
            _httpContextAccessor = httpContext;
        }

        protected HttpContext GetContext()
        {
            if ( _httpContextAccessor != null )
            {
                return _httpContextAccessor.HttpContext;
            }
            return null;
        }

        virtual public void RemoveSession()
        {
        }


        protected string GetConfigString(string keyName)
        {
            var context = GetContext();
            if ( context != null )
            {
                return context.Session.GetString(_sessionName + "." + keyName);
            }
            return null;
        }

        protected void SetConfigString(string keyName, string value)
        {
            var context = GetContext();
            if ( context != null )
            {
                if ( value != null )
                {
                    context.Session.SetString(_sessionName + "." + keyName, value);
                } 
                else
                {
                    context.Session.Remove(_sessionName + "." + keyName);
                }
            }
        }

        protected int? GetConfigInt(string keyName)
        {
            var context = GetContext();
            if ( context != null )
            {
                return context.Session.GetInt32(_sessionName + "." + keyName);
            }
            return null;
        }

        protected void SetConfigInt(string keyName, int? value)
        {
            var context = GetContext();
            if ( context != null )
            {
                if ( value != null )
                {
                    context.Session.SetInt32(_sessionName + "." + keyName, value.GetValueOrDefault());
                } 
                else
                {
                    context.Session.Remove(_sessionName + "." + keyName);
                }
            }
        }

        protected void RemoveConfig(string keyName)
        {
            var context = GetContext();
            if ( context != null )
            {
                context.Session.Remove(_sessionName + "." + keyName);
            }
        }
    }
}
