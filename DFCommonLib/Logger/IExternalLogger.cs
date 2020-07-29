using System;
namespace DFCommonLib.Logger
{
    public interface ILogOuputHandler
    {
        void LogMessage(OutputHandler.DFLogLevel logLevel, string group, string message);
    }
}
