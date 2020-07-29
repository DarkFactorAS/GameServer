using System;
using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using System.Linq;

namespace DFCommonLib.Logger
{
    public interface IDFLogger<T>
    {
        void LogInfo(string message);
        void LogWarning(string message);
    }

    public class OutputHandler
    {
        public enum DFLogLevel
        {
            Info,
            Warning,
            Error,
            Fatal,
        }

        public DFLogLevel logLevel;
        public ILogOuputHandler callback;

        public OutputHandler(DFLogLevel logLevel, ILogOuputHandler callback)
        {
            this.logLevel = logLevel;
            this.callback = callback;
        }
    }

    public class DFLogger<T> : IDFLogger<T>
    {
        private static IList<OutputHandler> _externalLogger;

        public DFLogger( )
        {
            _externalLogger = new List<OutputHandler>();
        }

        private string GetGroup()
        {
            return typeof(T).ToString();
        }

        public void LogInfo( string message )
        {
            LogOutput(OutputHandler.DFLogLevel.Warning, message );
        }

        public void LogWarning( string message )
        {
            LogOutput(OutputHandler.DFLogLevel.Warning, message );
        }

        public void LogError(string message)
        {
            LogOutput(OutputHandler.DFLogLevel.Warning, message);
        }

        public static void AddOutput(OutputHandler.DFLogLevel logLevel, ILogOuputHandler outputHandler )
        {
            var oldLogger = _externalLogger.Where(x => x == outputHandler).FirstOrDefault();
            if ( oldLogger == null )
            {
                _externalLogger.Add( new OutputHandler( logLevel, outputHandler) );
            }
        }

        private void LogOutput(OutputHandler.DFLogLevel logLevel, string messagge )
        {
            var group = GetGroup();
            foreach (OutputHandler outputHandler in _externalLogger)
            {
                if (outputHandler.logLevel <= logLevel && outputHandler.callback != null )
                {
                    outputHandler.callback.LogMessage(logLevel, group, messagge);
                }
            }
        }
    }
}
