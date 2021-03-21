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
using BotWebServer.Provider;

namespace BotWebServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class AccountController : ControllerBase
    {
        IAccountClient _accountClient;
        IBotSessionProvider _session;

        public AccountController(
            IAccountClient accountClient, 
            IConfigurationHelper configuration,
            IBotSessionProvider session )
        {
            _accountClient = accountClient;
            _session = session;

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
            var data = _accountClient.LoginAccount(loginData);
            if ( data.errorCode == AccountData.ErrorCode.OK )
            {
                _session.SetUser( data.nickname, data.token);
            }
            return data;
        }

        [HttpPut]
        [Route("CreateAccount")]
        public AccountData CreateAccount( CreateAccountData createAccountData )
        {
            var data = _accountClient.CreateAccount(createAccountData);
            if ( data.errorCode == AccountData.ErrorCode.OK )
            {
                _session.SetUser( data.nickname, data.token);
            }
            return data;
        }
    }
}
