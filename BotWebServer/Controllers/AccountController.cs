using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
//using AccountClientModule.Model;
using AccountClientModule.Client;
using AccountCommon.SharedModel;
using DFCommonLib.Config;
using DFCommonLib.HttpApi;
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
        IDeveloperProvider _developerProvider;

        public AccountController(
            IAccountClient accountClient,
            IDeveloperProvider developerProvider,
            IConfigurationHelper configuration,
            IBotSessionProvider session)
        {
            _accountClient = accountClient;
            _session = session;
            _developerProvider = developerProvider;

            var customer = configuration.GetFirstCustomer() as BotCustomer;
            if (customer != null)
            {
                _accountClient.SetEndpoint(customer.AccountServer);
            }
        }

        [HttpPut]
        [Route("LoginAccount")]
        public AccountData LoginAccount(LoginData loginData)
        {
            var data = _accountClient.LoginAccount(loginData);
            if (data.errorCode == AccountData.ErrorCode.OK)
            {
                _session.SetUser(data.nickname, data.token);
            }
            return data;
        }

        [HttpPut]
        [Route("LoginToken")]
        public AccountData LoginToken(LoginTokenData loginData)
        {
            var data = _accountClient.LoginToken(loginData);
            if (data.errorCode == AccountData.ErrorCode.OK)
            {
                _session.SetUser(data.nickname, data.token);
            }
            return data;
        }

        [HttpPut]
        [Route("SetDeveloper")]
        public int SetDeveloper(bool enable)
        {
            return _developerProvider.EnableDeveloper(enable);
        }

        [HttpPut]
        [Route("CreateAccount")]
        public AccountData CreateAccount(CreateAccountData createAccountData)
        {
            var data = _accountClient.CreateAccount(createAccountData);
            if (data.errorCode == AccountData.ErrorCode.OK)
            {
                _session.SetUser(data.nickname, data.token);
            }
            return data;
        }

        [HttpPut]
        [Route("ResetPasswordWithEmail")]
        public ReturnData ResetPasswordWithEmail(ResetPasswordDataEmail input)
        {
            var data = _accountClient.ResetPasswordWithEmail(input.emailAddress);
            return data;
        }

        [HttpPut]
        [Route("ResetPasswordWithCode")]
        public ReturnData ResetPasswordWithCode(ResetPasswordDataCode input)
        {
            var data = _accountClient.ResetPasswordWithCode(input.code);
            return data;
        }

        [HttpPut]
        [Route("ResetPasswordWithToken")]
        public ReturnData ResetPasswordWithToken(ResetPasswordDataToken input)
        {
            var data = _accountClient.ResetPasswordWithToken(input.password);
            return data;
        }

        [HttpPut]
        [Route("LoginGameCenterAccount")]
        public AccountData LoginGameCenterAccount(LoginData loginData)
        {
            return _accountClient.LoginGameCenter(loginData);
        }
    }
}
