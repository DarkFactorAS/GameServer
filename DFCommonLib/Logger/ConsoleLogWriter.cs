using System;
namespace DFCommonLib.Logger
{
    public class ConsoleLogWriter : ILogOutputWriter
    {
        public void LogMessage(DFLogLevel logLevel, string group, string message)
        {
            SetSeverityColor(logLevel);
            var logName = GetLogLevelName(logLevel);
            Console.WriteLine("[{0}][{1}] {2,-70}", logName, group, message);
        }
        public string GetName()
        {
            return "ConsoleLogWriter";
        }

        private static string GetLogLevelName(DFLogLevel logLevel)
        {
            switch (logLevel)
            {
                case DFLogLevel.UNKNOWN:
                case DFLogLevel.INFO:
                    return "info";
                case DFLogLevel.DEBUG:
                    return "dbug";
                case DFLogLevel.NOTIFICATION:
                    return "noti";
                case DFLogLevel.WARNING:
                    return "warn";
                case DFLogLevel.ERROR:
                    return "err ";
                case DFLogLevel.EXCEPTION:
                    return "exec";
                default:
                    return "";
            }
        }

        private static void SetSeverityColor(DFLogLevel logLevel)
        {
            switch (logLevel)
            {
                case DFLogLevel.UNKNOWN:
                case DFLogLevel.INFO:
                    SetColor(ConsoleColor.White);
                    break;
                case DFLogLevel.NOTIFICATION:
                    SetColor(ConsoleColor.Blue);
                    break;
                case DFLogLevel.DEBUG:
                case DFLogLevel.WARNING:
                    SetColor(ConsoleColor.DarkYellow);
                    break;
                case DFLogLevel.ERROR:
                case DFLogLevel.EXCEPTION:
                    SetColor(ConsoleColor.Red);
                    break;
            }
        }

        private static void SetColor(ConsoleColor color)
        {
            if (color == ConsoleColor.Black)
                color = ConsoleColor.White;
            Console.ForegroundColor = color;
        }
    }
}
