#include "CppUnitTest.h"
#include "user_service.h"
#include "mock_database.h"
#include "database_service.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UserServiceTests)
	{
	public:
		TEST_METHOD(insert_shortPassword_throwsRuntimeError)
		{
			MockDatabase db;
			UserService userService(db.getDatabase());

			CreateUserInfo user;
			user.username = "very_cool_username";
			user.insecurePassword = "pass";

			Assert::ExpectException<std::runtime_error>(
			[&]() {
				userService.insert(user);
			});
		}

		TEST_METHOD(insert_duplicateUsername_throwsRuntimeError)
		{
			MockDatabase db;
			UserService userService(db.getDatabase());

			CreateUserInfo user;
			user.username = "blahblahblah";
			user.insecurePassword = "password";

			// Insert the first time.
			userService.insert(user);

			Assert::ExpectException<std::runtime_error>(
			[&]() {
				// Should throw; user with that username already in datatabase :O
				userService.insert(user);
			});
		}

		TEST_METHOD(insert_validUsernameAndPassword_insertsUser)
		{
			MockDatabase db;
			UserService userService(db.getDatabase());

			CreateUserInfo newUser;
			newUser.username = "very_cool_username";
			newUser.insecurePassword = "password";
			newUser.email = "bill@123.com";
			newUser.dateOfBirth = "01/01/1970";
			userService.insert(newUser);

			LoginInfo login;
			login.username = newUser.username;
			login.insecurePassword = newUser.insecurePassword;
			User user = userService.login(login);

			Assert::AreEqual(newUser.username, user.username);
			Assert::AreEqual(newUser.email, user.email);
			Assert::AreEqual(newUser.dateOfBirth, user.dateOfBirth);
		}

		TEST_METHOD(login_badLoginDetails_throwsRuntimeException)
		{
			MockDatabase db;
			UserService userService(db.getDatabase());

			LoginInfo login;
			login.username = "i_do_not_exist";
			login.insecurePassword = "blahblahblah";

			Assert::ExpectException<std::runtime_error>(
			[&]() {
				userService.login(login);
			});
		}

		TEST_METHOD(login_sameUsernameAndPasswordAsInDatabase_returnsUser)
		{
			MockDatabase db;
			UserService userService(db.getDatabase());

			CreateUserInfo newUser;
			newUser.username = "thread0x23c8";
			newUser.insecurePassword = "password";
			newUser.email = "thread0x23c8@123.com";
			newUser.dateOfBirth = "01/01/1970";

			LoginInfo login;
			login.username = newUser.username;
			login.insecurePassword = newUser.insecurePassword;

			userService.insert(newUser);
			User user = userService.login(login);

			Assert::AreEqual(newUser.username, user.username);
			Assert::AreEqual(newUser.email, user.email);
			Assert::AreEqual(newUser.dateOfBirth, user.dateOfBirth);
		}
	};
}
