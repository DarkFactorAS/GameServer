using System;
using Microsoft.Extensions.DependencyInjection;
using DFCommonLib.DataAccess;
using DFCommonLib.Config;
using DFCommonLib.Logger;

namespace DFCommonLib.Utils
{
    public class DFServices
    {
        IServiceCollection _services;

        public DFServices(IServiceCollection services)
        {
            _services = services;
        }

        public DFServices SetupLogger()
        {
            // Add all template variants
            _services.AddSingleton(typeof(IDFLogger<>), typeof(DFLogger<>));
            _services.AddScoped<IConfigurationHelper, ConfigurationHelper>();
            return this;
        }

        public DFServices SetupMySql()
        {
            _services.AddScoped<IDbConnectionFactory, LocalMysqlConnectionFactory>();
            return this;
        }

        public DFServices Setup()
        {
            SetupLogger();
            SetupMySql();
            return this;
        }

        public DFServices LogToConsole(DFLogLevel logLevel)
        {
            DFLogger.AddOutput(logLevel, new ConsoleLogWriter());
            return this;
        }

        public DFServices LogToMySQL(DFLogLevel logLevel)
        {
            var serviceProvider = _services.BuildServiceProvider();
            var connection = (IDbConnectionFactory)serviceProvider.GetService(typeof(IDbConnectionFactory));
            if (connection != null )
            {
                DFLogger.AddOutput(logLevel, new MySqlLogWriter(connection));
            }
            return this;
        }

        public DFServices LogToEvent(DFLogLevel logLevel, string appName)
        {
            DFLogger.AddOutput(logLevel, new EventLogWriter(appName));
            return this;
        }

    }
}
