#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MouseCraft/Address.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NetworkTests {		
    TEST_CLASS(AddressTests) {
    public:
        TEST_METHOD(SplitConstructor) {
            Address a(128, 255, 64, 32, 0);
            Assert::AreEqual(a.GetAddress(), 0x80FF4020);
        }

        TEST_METHOD(GetA) {
            Address a(0x80FFFFFF, 0);
            Assert::AreEqual(a.GetA(), (unsigned char)128);
        }

        TEST_METHOD(GetB) {
            Address a(0xFF80FFFF, 0);
            Assert::AreEqual(a.GetB(), (unsigned char)128);
        }

        TEST_METHOD(GetC) {
            Address a(0xFFFF80FF, 0);
            Assert::AreEqual(a.GetC(), (unsigned char)128);
        }

        TEST_METHOD(GetD) {
            Address a(0xFFFFFF80, 0);
            Assert::AreEqual(a.GetD(), (unsigned char)128);
        }
    };
}