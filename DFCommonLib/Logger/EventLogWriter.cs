using System;
using DFCommonLib.DataAccess;
using System.Diagnostics;

namespace DFCommonLib.Logger
{
    public class EventLogWriter : ILogOutputWriter
    {
        private string _appName;

        public EventLogWriter(string appName)
        {
            _appName = appName;
        }

        public string GetName()
        {
            return "EventLogWriter";
        }

        public void LogMessage(DFLogLevel logLevel, string group, string message)
        {
            // Create node if it does not exist
            if (!EventLog.SourceExists(_appName))
            {
                EventLog.CreateEventSource(_appName, "Application");
            }

            var eventLogEntryType = GetLogEntryType(logLevel);
            EventLog.WriteEntry(_appName, message, eventLogEntryType);
        }

        private EventLogEntryType GetLogEntryType(DFLogLevel logLevel)
        {
            return EventLogEntryType.Error;
        }
    }
}
