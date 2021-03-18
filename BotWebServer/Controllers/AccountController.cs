using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using AccountClientModule.Model;
using AccountClientModule.Client;
using DFCommonLib.Config;
using BotWebServer.Model;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class AccountController : ControllerBase
    {
        ILogger<AccountController> _logger;
        IAccountClient _accountClient;

        public AccountController(ILogger<AccountController> logger, IAccountClient accountClient, IConfigurationHelper configuration)
        {
            _logger = logger;
            _accountClient = accountClient;

            var customer = configuration.GetFirstCustomer() as BotCustomer;
            if ( customer != null )
            {
                _accountClient.SetEndpoint(customer.AccountServer);
            }
        }

        [HttpPut]
        [Route("LoginAccount")]
        public AccountData LoginAccount(LoginData loginData)
        {
            return _accountClient.LoginAccount(loginData);
        }

        [HttpPut]
        [Route("CreateAccount")]
        public AccountData CreateAccount( CreateAccountData createAccountData )
        {
            return _accountClient.CreateAccount(createAccountData);
        }
    }
}
