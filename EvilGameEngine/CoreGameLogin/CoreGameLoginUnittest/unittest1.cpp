#include "PreCompile.h"
#include "CppUnitTest.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"

// https://docs.microsoft.com/en-us/visualstudio/test/writing-unit-tests-for-c-cpp

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreGameLoginUnittest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(VerifyAccountData)
		{
		      // TODO: Your test code here
                      Account* newAccount = new Account();
                      int retCode = AccountManager::VerifyAccountData(newAccount);

                      Assert::AreEqual(retCode, 0);
		}

	};
}