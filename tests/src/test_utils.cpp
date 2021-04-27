#include "CppUnitTest.h"
#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(UtilsTests)
    {
    public:
        TEST_METHOD(split_sentance_returnsArrayOfWords)
        {
            std::vector<std::string> words = Utils::split("I Do Not Like Green Eggs and Ham", " ");
            
            Assert::AreEqual(8, (int)words.size());
            Assert::AreEqual("I"    , words[0].c_str());
            Assert::AreEqual("Do"   , words[1].c_str());
            Assert::AreEqual("Not"  , words[2].c_str());
            Assert::AreEqual("Like" , words[3].c_str());
            Assert::AreEqual("Green", words[4].c_str());
            Assert::AreEqual("Eggs" , words[5].c_str());
            Assert::AreEqual("and"  , words[6].c_str());
            Assert::AreEqual("Ham"  , words[7].c_str());
        }

        TEST_METHOD(toDecimalPlaces_double_returnsStringRepresentationToSpecifiedDecimalPlaces)
        {
            std::string result = Utils::toDecimalPlaces(42.444, 2);
            Assert::AreEqual("42.44", result.c_str());
        }

        TEST_METHOD(parseInt_inputNotANumber_returnsNullopt)
        {
            std::optional<int> result = Utils::parseInt("This is definitely not a number");
            Assert::IsFalse((bool)result);
        }

        TEST_METHOD(parseInt_inputIsANumber_returnsNumber)
        {
            std::optional<int> result = Utils::parseInt("42");
            Assert::IsTrue((bool)result);
            Assert::AreEqual(42, *result);
        }
    };
}
