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
using DFCommonLib.Utils;

namespace BotWebServer
{
    public class Program
    {
        public static void Main(string[] args)
        {
            CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureServices((hostContext, services) =>
                {
                    services.AddSingleton(typeof(IDFLogger<>), typeof(DFLogger<>));

                    //DFCommonLib.Utils.DFCommonLib.SetupLogger(services);
                    DFCommonLib.Utils.DFCommonLib.SetupMySql(services);

                    services.AddTransient<IPlayfieldRepository, PlayfieldRepository>();
                    services.AddTransient<IConfigurationHelper, ConfigurationHelper>();

                })
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>();
                });
    }
}
