#include "pch.h"
#include "CppUnitTest.h"
#include "Matrix.h"
#include "MatrixX.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(tests)
	{
	public:
		
		// Addition of two matrices.
		TEST_METHOD(UnitTest1_MatrixAddition)
		{
			MatrixXi m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXi m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};

			MatrixXi actual = m1 + m2;
			MatrixXi expected{
				{3, 1, 1},
				{3, 0, 1},
				{1, 1, 2}
			};
			Assert::IsTrue(actual == expected);
		}

		// Subtraction of matrices.
		TEST_METHOD(UnitTest2_MatrixSubtraction)
		{
			MatrixXi m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXi m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};

			MatrixXi actual = m2 - m1;
			MatrixXi expected{
				{1, 1, 1},
				{3, -2, 1},
				{1, 1, 0}
			};
			Assert::IsTrue(actual == expected);
		}

		// Multiplication of matrices.
		TEST_METHOD(UnitTest3_MatrixMultiplication)
		{
			MatrixXi m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXi m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};

			MatrixXi actual = m2 * m1;
			MatrixXi expected{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};
			Assert::IsTrue(actual == expected);
		}

		// Scalar multiplication
		TEST_METHOD(UnitTest4_ScalarMultiplication)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			const double k{ 2 };

			MatrixXd actual = k * m1;
			MatrixXd expected{
				{2, 0, 0},
				{0, 2, 0},
				{0, 0, 2}
			};
			Assert::IsTrue(actual == expected);
		}
	};
}
