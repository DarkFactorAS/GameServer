using System;
using Microsoft.Extensions.DependencyInjection;
using DFCommonLib.DataAccess;
using DFCommonLib.Config;

namespace DFCommonLib.Utils
{
    public class DFCommonLib
    {
        public static void SetupLogger(IServiceCollection services)
        {
            // Add all template variants
            services.AddSingleton(typeof(IDFLogger<>), typeof(DFLogger<>));
            services.AddScoped<IConfigurationHelper, ConfigurationHelper>();
        }

        public static void SetupMySql(IServiceCollection services)
        {
            services.AddScoped<IDbConnectionFactory, LocalMysqlConnectionFactory>();
        }
    }
}
