using System;
using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using System.Linq;

namespace DFCommonLib.Logger
{
    public interface IDFLogger<T>
    {
        void Startup();
        void LogDebug(string message);
        void LogInfo(string message);
        void LogWarning(string message);
        void LogError(string message);
    }

    public class OutputWriter
    {
        public DFLogLevel logLevel;
        public ILogOutputWriter logOutputWriter;

        public OutputWriter(DFLogLevel logLevel, ILogOutputWriter logOutputWriter)
        {
            this.logLevel = logLevel;
            this.logOutputWriter = logOutputWriter;
        }
    }

    public class DFLogger<T> : IDFLogger<T>
    {
        private string GetGroup()
        {
            return typeof(T).ToString();
        }

        private string GetClassName()
        {
            var groupName = GetGroup();
            var lastGroup = groupName.Split(".").LastOrDefault();
            if ( lastGroup != null )
            {
                return lastGroup;
            }
            return groupName;
        }

        public void Startup()
        {
            var group = GetClassName();
            var message = "Init application";
            DFLogger.PrintStartup(DFLogLevel.INFO, group,message);

            LogInfo("******************************************");    
            LogInfo("***                                    ***");    
            LogInfo("***  Starting Bot WebServer            ***");    
            LogInfo("***                                    ***");    
            LogInfo("******************************************");    
        }

        public void LogInfo( string message )
        {
            var group = GetClassName();
            DFLogger.LogOutput(DFLogLevel.INFO, group, message );
        }

        public void LogDebug( string message )
        {
            #if DEBUG
                var group = GetClassName();
                DFLogger.LogOutput(DFLogLevel.DEBUG, group, message );
            #endif
        }

        public void LogWarning( string message )
        {
            var group = GetClassName();
            DFLogger.LogOutput(DFLogLevel.WARNING, group, message );
        }

        public void LogError(string message)
        {
            var group = GetClassName();
            DFLogger.LogOutput(DFLogLevel.ERROR, group, message);
        }
    }

    public class DFLogger
    {
        private static IList<OutputWriter> _ouputWriters = new List<OutputWriter>();

        public static void AddOutput(DFLogLevel logLevel, ILogOutputWriter outputWriter)
        {
            var oldLogger = _ouputWriters.Where(x => x == outputWriter).FirstOrDefault();
            if (oldLogger == null)
            {
                _ouputWriters.Add(new OutputWriter(logLevel, outputWriter));
            }
        }

        public static void LogOutput(DFLogLevel logLevel, string group, string message)
        {
            foreach (OutputWriter outputWriter in _ouputWriters)
            {
                if (outputWriter.logLevel <= logLevel && outputWriter.logOutputWriter != null)
                {
                    try
                    {
                        outputWriter.logOutputWriter.LogMessage(logLevel, group, message);
                    }
                    catch(System.PlatformNotSupportedException ex)
                    {
                        outputWriter.logLevel = DFLogLevel.DISABLED;
                        LogOutput(DFLogLevel.ERROR, "DFLogger", string.Format("Removing {0} due to : {1} ", outputWriter.logOutputWriter.GetName(), ex.ToString()));
                    }
                    catch(Exception ex)
                    {
                        // Temp disable this and try to log error to other outputs
                        var tmpLogLevel = outputWriter.logLevel;
                        outputWriter.logLevel = DFLogLevel.DISABLED;
                        LogOutput(DFLogLevel.EXCEPTION, "DFLogger", string.Format("{0}:{1}", outputWriter.logOutputWriter.GetName(), ex.ToString()));
                        outputWriter.logLevel = tmpLogLevel;
                    }
                }
            }
        }

        public static void PrintStartup(DFLogLevel logLevel, string group, string message)
        {
            foreach (OutputWriter outputWriter in _ouputWriters)
            {
                if (outputWriter.logOutputWriter != null)
                {
                    try
                    {
                        outputWriter.logOutputWriter.LogMessage(logLevel, group, message);
                    }
                    catch(System.PlatformNotSupportedException ex)
                    {
                        outputWriter.logLevel = DFLogLevel.DISABLED;
                        LogOutput(DFLogLevel.ERROR, "DFLogger", string.Format("Removing {0} due to : {1} ", outputWriter.logOutputWriter.GetName(), ex.ToString()));
                    }
                    catch(Exception ex)
                    {
                        // Temp disable this and try to log error to other outputs
                        var tmpLogLevel = outputWriter.logLevel;
                        outputWriter.logLevel = DFLogLevel.DISABLED;
                        LogOutput(DFLogLevel.EXCEPTION, "DFLogger", string.Format("{0}:{1}", outputWriter.logOutputWriter.GetName(), ex.ToString()));
                        outputWriter.logLevel = tmpLogLevel;
                    }
                }
            }
        }
    }
}
