#include "CppUnitTest.h"
#include "cart.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(CartTests)
	{
	public:
		TEST_METHOD(getIsEmpty_noOrders_returnsTrue)
		{
			Cart cart;
			Assert::IsTrue(cart.getIsEmpty());
		}

		TEST_METHOD(getIsEmpty_hasOrders_returnsFalse)
		{
			Cart cart;
			cart.addOrder((Cart::GameID)2, 10);
			Assert::IsFalse(cart.getIsEmpty());
		}

		TEST_METHOD(clear_hasOrders_clearsOrders)
		{
			Cart cart;
			cart.addOrder((Cart::GameID)1, 10);
			cart.addOrder((Cart::GameID)2, 10);

			cart.clear();

			Assert::IsTrue(cart.getIsEmpty());
		}

		TEST_METHOD(getCount_hasOrder_returnsQuantityOfOrdersForGame)
		{
			constexpr auto gameID = 42;
			constexpr auto quantity = 10;

			Cart cart;
			cart.addOrder(gameID, quantity);

			Assert::AreEqual(quantity, cart.getCount(gameID));

			// Also test adding a single copy, without a quantity argument.
			cart.addOrder(gameID); 
			Assert::AreEqual(quantity + 1, cart.getCount(gameID));
		}
	};
}
