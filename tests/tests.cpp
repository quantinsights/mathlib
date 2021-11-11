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
		// Basic linear algebra
		/// <summary>
		/// A + B
		/// </summary>
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

		TEST_METHOD(UnitTest1b_ChainedAddition)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd m2{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd m3{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};


			MatrixXd actual = m1 + m2 + m3;
			MatrixXd expected{
				{3, 0, 0},
				{0, 3, 0},
				{0, 0, 3}
			};
			Assert::IsTrue(actual == expected);
		}

		// A - B
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

		/// <summary>
		/// \f$ A \times B \f$
		/// </summary>
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

		/// <summary>
		/// \f$ k \cdot A \f$
		/// </summary>
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

		/// <summary>
		/// Unary plus operator +A
		/// </summary>
		TEST_METHOD(UnitTest5_UnaryPlus)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd actual = +m1;
			MatrixXd expected{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};
			Assert::IsTrue(actual == expected);
		}

		/// <summary>
		/// Unary minus operator -A
		/// </summary>
		TEST_METHOD(UnitTest6_UnaryMinus)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd actual = -m1;
			MatrixXd expected{
				{-1, 0, 0},
				{0, -1, 0},
				{0, 0, -1}
			};
			Assert::IsTrue(actual == expected);
		}

		/// <summary>
		/// Computed assignments
		/// A += B
		/// </summary>
		TEST_METHOD(UnitTest7_AdditionAssignment)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};
			MatrixXd actual = (m1+=m2);
			MatrixXd expected{
				{3, 1, 1},
				{3, 0, 1},
				{1, 1, 2}
			};
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(UnitTest8_SubtractionAssignment)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};
			MatrixXd actual = (m1 -= m2);
			MatrixXd expected{
				{-1, -1, -1},
				{-3, 2, -1},
				{-1, -1, 0}
			};
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(UnitTest9_MultiplicationAssignment)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd m2{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};
			MatrixXd actual = (m1 *= m2);
			MatrixXd expected{
				{2, 1, 1},
				{3, -1, 1},
				{1, 1, 1}
			};
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(UnitTest10_rowVectori)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd actual = row(m1, 1);
			MatrixXd expected{
				{0, 1, 0}
			};
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(UnitTest11_colVectorj)
		{
			MatrixXd m1{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};

			MatrixXd actual = col(m1, 1);
			MatrixXd expected{
				{0},
				{1},
				{0}
			};
			Assert::IsTrue(actual == expected);
		}
	};
}
