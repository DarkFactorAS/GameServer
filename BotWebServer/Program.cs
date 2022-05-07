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

        public static void Main(string[] args)
        {
            var builder = CreateHostBuilder(args).Build();

            try
            {
                // Run database script
                IStartupRepository startupRepository = DFServices.GetService<IStartupRepository>();
                startupRepository.RunPatcher();

                // Set adress to account server
                IConfigurationHelper configuration = DFServices.GetService<IConfigurationHelper>();
                IAccountClient client = DFServices.GetService<IAccountClient>();
                var customer = configuration.GetFirstCustomer() as BotCustomer;
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
