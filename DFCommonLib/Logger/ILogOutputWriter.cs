using System;
namespace DFCommonLib.Logger
{
    public interface ILogOutputWriter
    {
        void LogMessage(DFLogLevel logLevel, string group, string message);
        string GetName();
    }
}
