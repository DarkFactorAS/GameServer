using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.HttpsPolicy;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Swashbuckle.AspNetCore.Swagger;

using DFCommonLib.DataAccess;
using DFCommonLib.Utils;
using DFCommonLib.Logger;
using AccountClientModule.Client;

using BotWebServer.Repository;
using BotWebServer.Provider;

using DFCommonLib.Config;

namespace BotWebServer
{

    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;

            IDFLogger<Startup> logger = new DFLogger<Startup>();
            logger.Startup(Program.AppName, Program.Version);
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            DFServices.Create(services);

            services.AddControllers();
            services.AddMvc();

            // register the swagger generator
            services.AddSwaggerGen();

            services.AddSession(options =>
            {
                // Set a short timeout for easy testing.
                options.IdleTimeout = TimeSpan.FromMinutes(20);
                options.Cookie.HttpOnly = true;
                // Make the session cookie essential
                options.Cookie.IsEssential = true;
            });

            // services.AddTransient<IPlayfieldRepository, PlayfieldRepository>();
            // services.AddTransient<IPlayfieldProvider, PlayfieldProvider>();

            services.AddHttpContextAccessor();

            //services.AddSingleton<IHttpContextAccessor, HttpContextAccessor>();
            services.AddTransient(typeof(IStartupDatabasePatcher), typeof(BotDatabasePatcher));
            services.AddTransient(typeof(IPlayfieldRepository), typeof(PlayfieldRepository));
            services.AddTransient(typeof(IPlayfieldProvider), typeof(PlayfieldProvider));
            services.AddScoped(typeof(IBotSessionProvider), typeof(BotSessionProvider));
            AccountClient.SetupService(services);
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseHttpsRedirection();
            app.UseRouting();
            app.UseAuthorization();
            app.UseSession();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });

            app.UseSwagger();

            // specify the Swagger JSON endpoint.
            app.UseSwaggerUI(c =>
            {
                c.RoutePrefix = "";
                c.SwaggerEndpoint("/swagger/v1/swagger.json", "My API V1");
            });
        }
    }
}
