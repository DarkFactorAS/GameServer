using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using BotWebServer.Repository;
using BotWebServer.Model;

using DFCommonLib.DataAccess;
using DFCommonLib.Config;
using DFCommonLib.Logger;
using DFCommonLib.Utils;
using AccountClientModule.Client;

using BotWebServer.Provider;

namespace BotWebServer
{
    public class Program
    {
        public static string AppName = "Bot WebServer";
        public static string Version = "0.9.3";

        public static void Main(string[] args)
        {
            var builder = CreateHostBuilder(args).Build();

            try
            {
                IConfigurationHelper configuration = DFServices.GetService<IConfigurationHelper>();
                var customer = configuration.GetFirstCustomer() as BotCustomer;
                var msg = string.Format("Connecting to DB : {0}", customer.DatabaseConnections.FirstOrDefault()?.ConnectionString);
                DFLogger.LogOutput(DFLogLevel.INFO, "BotServer", msg);

                // Run database script
                IStartupDatabasePatcher startupRepository = DFServices.GetService<IStartupDatabasePatcher>();
                startupRepository.WaitForConnection();
                if (startupRepository.RunPatcher() )
                {
                    DFLogger.LogOutput(DFLogLevel.INFO, "Startup", "Database patcher ran successfully" );
                }
                else
                {
                    DFLogger.LogOutput(DFLogLevel.ERROR, "Startup", "Database patcher failed" );
                    Environment.Exit(1);
                    return;                    
                }

                // Set adress to account server
                IAccountClient client = DFServices.GetService<IAccountClient>();
                if ( customer != null )
                {
                    client.SetEndpoint(customer.AccountServer);
                }

                // Make sure we have connection to database
                DFLogger.LogOutput(DFLogLevel.INFO, "BotServer", "AccountServer:PING" );
                var result = client.PingServer();
                DFLogger.LogOutput(DFLogLevel.INFO, "BotServer", "AccountServer:" + result );

                builder.Run();
            }
            catch( Exception ex )
            {
                DFLogger.LogOutput(DFLogLevel.WARNING, "Startup", ex.ToString() );
            }
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureServices((hostContext, services) =>
                {
                    services.AddTransient<IConfigurationHelper, BotConfigurationHelper>();

                    new DFServices(services)
                        .SetupLogger()
                        .SetupMySql()
                        .LogToConsole(DFLogLevel.INFO)
                        .LogToMySQL(DFLogLevel.WARNING)
                        .LogToEvent(DFLogLevel.ERROR, AppName);
                        ;

                })
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>();
                }
        );
    }
}
