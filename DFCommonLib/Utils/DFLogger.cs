using System;
using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using System.Linq;

namespace DFCommonLib.Utils
{
    public interface IDFLogger<T>
    {
        void LogInfo(string message);
        void LogWarning(string message);
        void AddExternal(ExternalLogger.DFLogLevel logLevel, IExternalLogger externalLogger);
    }

    public class ExternalLogger
    {
        public enum DFLogLevel
        {
            Info,
            Warning
        }

        public DFLogLevel logLevel;
        public IExternalLogger callback;

        public ExternalLogger(DFLogLevel logLevel, IExternalLogger callback)
        {
            this.logLevel = logLevel;
            this.callback = callback;
        }
    }

    public class DFLogger<T> : IDFLogger<T>
    {
        private readonly ILogger<T> _logger;
        private IList<ExternalLogger> _externalLogger;

        public DFLogger( ILogger<T> logger )
        {
            _logger = logger;
            _externalLogger = new List<ExternalLogger>();
        }

        private string GetGroup()
        {
            return typeof(T).ToString();
        }

        public void LogInfo( string message )
        {
            _logger.LogInformation( message );
            LogExternal(ExternalLogger.DFLogLevel.Warning, message );
        }

        public void LogWarning( string message )
        {
            _logger.LogWarning( message );
            LogExternal(ExternalLogger.DFLogLevel.Warning, message );
        }

        public void LogError(string message)
        {
            _logger.LogError(message);
            LogExternal(ExternalLogger.DFLogLevel.Warning, message);
        }

        public void AddExternal(ExternalLogger.DFLogLevel logLevel, IExternalLogger externalLogger )
        {
            var oldLogger = _externalLogger.Where(x => x == externalLogger).FirstOrDefault();
            if ( oldLogger == null )
            {
                _externalLogger.Add( new ExternalLogger( logLevel, externalLogger ) );
            }
        }

        private void LogExternal(ExternalLogger.DFLogLevel logLevel, string messagge )
        {
            var group = GetGroup();
            foreach (ExternalLogger externalLogger in _externalLogger)
            {
                if (externalLogger.logLevel <= logLevel && externalLogger.callback != null )
                {
                    externalLogger.callback.LogMessage(logLevel, group, messagge);
                }
            }
        }
    }
}
