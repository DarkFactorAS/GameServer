using System;
namespace DFCommonLib.Utils
{
    public interface IExternalLogger
    {
        void LogMessage(ExternalLogger.DFLogLevel logLevel, string group, string message);
    }
}
