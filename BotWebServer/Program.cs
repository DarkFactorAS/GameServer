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

using DFCommonLib.Config;
using DFCommonLib.Logger;
using DFCommonLib.Utils;

using BotWebServer.Provider;
using AccountClientModule.Client;

namespace BotWebServer
{
    public class Program
    {
        public static string AppName = "Bot WebServer";

        public static void Main(string[] args)
        {
            CreateHostBuilder(args).Build().Run();
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

                    services.AddTransient<IPlayfieldRepository, PlayfieldRepository>();
                    services.AddTransient<IPlayfieldProvider, PlayfieldProvider>();
                    services.AddTransient<IBotSessionProvider, BotSessionProvider>();

                    AccountClient.SetupService(services);
                })
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>();
                }
        );
    }
}
