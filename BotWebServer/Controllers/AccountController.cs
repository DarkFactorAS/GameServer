using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using AccountClientModule.Client;
using AccountCommon.SharedModel;
using DFCommonLib.Config;
using DFCommonLib.HttpApi;
using BotWebServer.Model;
using BotWebServer.Provider;
using DFCommonLib.HttpApi.OAuth2;
using Microsoft.AspNetCore.Authorization;

namespace BotWebServer.Controllers
{
    [Authorize(AuthenticationSchemes = OAuth2Static.AuthenticationScheme)]
    public class AccountController : DFRestOAuth2ServerController
    {
        IAccountClient _accountClient;
        IBotSessionProvider _session;
        IDeveloperProvider _developerProvider;

        public AccountController(
            IAccountClient accountClient,
            IDeveloperProvider developerProvider,
            IConfigurationHelper configurationHelper,
            IBotSessionProvider session)
        {
            _accountClient = accountClient;
            _session = session;
            _developerProvider = developerProvider;

            var customer = configurationHelper.Settings as BotConfig;
            if (customer != null)
            {
                _accountClient.SetEndpoint(customer.AccountServer.Endpoint);
                _accountClient.SetAuthCredentials(customer.AccountServer.ClientId, customer.AccountServer.ClientSecret, customer.AccountServer.Scope);
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
            if (input.emailAddress == null || string.IsNullOrWhiteSpace(input.emailAddress))
            {
                return new ReturnData
                {
                    errorCode = (int)ReturnData.ReturnCode.ErrorInData,
                    message = "emailAddress is required."
                };
            }
            var data = _accountClient.ResetPasswordWithCode(input.code, input.emailAddress);
            return data;
        }

        [HttpPut]
        [Route("ResetPasswordWithToken")]
        public ReturnData ResetPasswordWithToken(ResetPasswordDataToken input)
        {
            if (input == null || string.IsNullOrEmpty(input.token) || string.IsNullOrEmpty(input.password))
            {
                return new ReturnData
                {
                    errorCode = (int)ReturnData.ReturnCode.ErrorInData,
                    message = "Token and password are required."
                };
            }
            var data = _accountClient.ResetPasswordWithToken(input.token, input.password);
            return data;
        }

        [HttpPut]
        [Route("LoginGameCenterAccount")]
        public AccountData LoginGameCenterAccount(LoginGameCenterData loginData)
        {
            return _accountClient.LoginGameCenter(loginData);
        }
    }
}
