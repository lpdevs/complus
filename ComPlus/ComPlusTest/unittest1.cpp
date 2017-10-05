#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../ComPlus/Common/SQueue.h"

namespace ComPlusTest
{		
	TEST_CLASS(SQueueTest)
	{
	public:
		TEST_METHOD(Constructor)
		{
			SQueue<int> *mQueue = new SQueue<int>();
			Assert::AreEqual(0, mQueue->size(), L"Init success", LINE_INFO());
		}
		TEST_METHOD(Opertion)
		{
			SQueue<int> *mQueue = new SQueue<int>();
			mQueue->push(10);
			mQueue->push(12);
			Assert::AreEqual(2, mQueue->size(), L"Check size success", LINE_INFO());
			Assert::AreEqual(10, mQueue->front(), L"Front success", LINE_INFO());
			Assert::AreEqual(2, mQueue->size(), L"Check size success", LINE_INFO());
			Assert::AreEqual(10, mQueue->pop(), L"Pop success", LINE_INFO());
			Assert::AreEqual(1, mQueue->size(), L"Check size success", LINE_INFO());
		}
	};
}