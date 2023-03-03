#pragma once
#include "UnitTestHeader.h"
#include "Core/DataType/SharedObject.h"

namespace Eggy::UnitTest
{
	namespace SharedObject_UnitTest
	{
		namespace SharePtr_UnitTest
		{
			static bool NotSharedClassIsExpired = false;
			class NotSharedClass
			{
			public:
				NotSharedClass()
				{
					NotSharedClassIsExpired = false;
				}

				~NotSharedClass()
				{
					NotSharedClassIsExpired = true;
				}

				int I{ 1 };
			};

			class SharedClass
			{
			public:
				void Acquire() { mRef_++; }
				void Release() { mRef_--; }
				int GetRefCount()
				{
					return mRef_;
				}

			protected:
				int mRef_{ 0 };
			};

			static void TestEnableShared()
			{
				auto ptr = new SharedClass();
				{
					TSharedPtr<SharedClass> ptr1(ptr);
					TSharedPtr<SharedClass> ptr2 = ptr1;
					TSharedPtr<SharedClass> ptr3(std::move(ptr1));
					TSharedPtr<SharedClass> ptr4(ptr2);
					ptr1 = std::move(ptr2);
					UT_ASSERT(!ptr2);
					UT_ASSERT(ptr1.GetRefCount());
					UT_ASSERT(ptr->GetRefCount() == 3);
				}
				UT_ASSERT(ptr->GetRefCount() == 0);
			}

			static void TestDisenableShared()
			{
				auto ptr = new NotSharedClass();
				{
					TSharedPtrUnref<NotSharedClass> ptr1(ptr);
					TSharedPtrUnref<NotSharedClass> ptr2 = ptr1;
					TSharedPtrUnref<NotSharedClass> ptr3(std::move(ptr1));
					TSharedPtrUnref<NotSharedClass> ptr4(ptr2);
					ptr1 = std::move(ptr2);
					UT_ASSERT(!ptr2);
					UT_ASSERT(ptr3->I == 1);
					UT_ASSERT(ptr3.GetRefCount() == 3);
				}
				UT_ASSERT(NotSharedClassIsExpired); // now ptr is invalid
			}
		}
		

		static void Test()
		{
			SharePtr_UnitTest::TestEnableShared();
			SharePtr_UnitTest::TestDisenableShared();
		}
	}
}


