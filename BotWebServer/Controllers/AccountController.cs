using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using AccountClientModule.Model;
using AccountClientModule.Client;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class AccountController : ControllerBase
    {
        ILogger<AccountController> _logger;
        IAccountClient _accountClient;

        public AccountController(ILogger<AccountController> logger, IAccountClient accountClient)
        {
            _logger = logger;
            _accountClient = accountClient;
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
