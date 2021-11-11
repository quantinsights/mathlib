#pragma once
#ifndef Matrix_H
#include <array>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <iomanip>
#include <initializer_list>
#include <algorithm>

template <typename T, int r = 0, int c = 0>
class Matrix;

using Matrix1d = Matrix<double, 1, 1>;
using Matrix2d = Matrix<double, 2, 2>;
using Matrix3d = Matrix<double, 3, 3>;
using Matrix4d = Matrix<double, 4, 4>;

using Matrix1i = Matrix<int, 1, 1>;
using Matrix2i = Matrix<int, 2, 2>;
using Matrix3i = Matrix<int, 3, 3>;
using Matrix4i = Matrix<int, 4, 4>;

using Matrix1f = Matrix<float, 1, 1>;
using Matrix2f = Matrix<float, 2, 2>;
using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;

using Vector1d = Matrix<double, 1, 1>;
using Vector2d = Matrix<double, 1, 2>;
using Vector3d = Matrix<double, 1, 3>;
using Vector4d = Matrix<double, 1, 4>;

using Vector1i = Matrix<int, 1, 1>;
using Vector2i = Matrix<int, 1, 2>;
using Vector3i = Matrix<int, 1, 3>;
using Vector4i = Matrix<int, 1, 4>;

using Vector1f = Matrix<float, 1, 1>;
using Vector2f = Matrix<float, 1, 2>;
using Vector3f = Matrix<float, 1, 3>;
using Vector4f = Matrix<float, 1, 4>;



/// <summary>
/// Class template for fixed-size matrices.
/// </summary>
/// <typeparam name="scalarType">Scalar type of the elements in the matrix</typeparam>
/// <typeparam name="rowsAtCompileTime">Number of rows</typeparam>
/// <typeparam name="colsAtCompileTime">Number of columns</typeparam>
template <typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
class Matrix
{
private:
	std::array<scalarType, rowsAtCompileTime* colsAtCompileTime> A;
	int _rows;
	int _cols;
	int _size;
	typename std::array<scalarType, rowsAtCompileTime* colsAtCompileTime>::iterator currentPosition;
public:
	Matrix();
	Matrix(int m, int n);
	Matrix(const Matrix& m);
	Matrix(std::initializer_list<std::initializer_list<scalarType>>);

	int rows() const;
	int cols() const;
	int size() const;

	//Overloaded operators
	scalarType operator()(const int i, const int j) const;		//Subscript operator
	scalarType& operator()(const int i, const int j);			//Subscript operator const arrays
	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix& operator<<(const scalarType x);
	Matrix& operator,(const scalarType x);
	Matrix& operator=(const Matrix& right_hand_side);
	bool operator==(const Matrix& right_hand_side);
	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	
	//Matrix& transpose();

};

// Global operators
template<typename scalarType, int m, int n, int p, int q>
Matrix<scalarType, m, q> operator*(const Matrix<scalarType, m, n>& A, const Matrix<scalarType, p, q>& B);


template<typename scalarType, int m, int n>
Matrix<scalarType, m, n> operator*(const scalarType k, const Matrix<scalarType, m, n>& mat);

// Non-member operator functions
template<typename scalarType, int m, int n, int p, int q>
Matrix<scalarType, m, q>& operator*=(Matrix<scalarType, m, n>& A, const Matrix<scalarType, p, q>& B);


template<typename scalarType, int m, int n>
Matrix<scalarType, m, n>& operator*=(const scalarType k, Matrix<scalarType, m, n>& mat);

/// <summary>
/// The default constructor.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::Matrix() :_rows{ rowsAtCompileTime }, _cols{ colsAtCompileTime }, _size{ rowsAtCompileTime * colsAtCompileTime }
{
	currentPosition = A.begin();
}

/// <summary>
/// For fixed-size matrices, the arguments ``m`` and ``n`` are ignored, and instead 
/// the dimensions of the matrix are determined from the template parameters
/// ``rowsAtCompileTime`` and ``colsAtCompileTime``.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <param name="n"></param>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::Matrix(int m, int n)
{
	//Do nothing.
}

/// <summary>
/// Copy constructor.
/// Creates a deep copy of the Matrix object passed.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::Matrix(const Matrix& m) : A{ m.A }, _rows{ m.rows() }, _cols{ m.cols() }, currentPosition{ m.currentPosition }
{
}

/// <summary>
/// Allocates and initializes a matrix using the curly brace initializer list.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="list"></param>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::Matrix(std::initializer_list<std::initializer_list<scalarType>> list) :Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>{}	//Delegate to the default constructor to set up the initial array
{
	// Initialize the array from our list.
	typename std::array<scalarType, rowsAtCompileTime* colsAtCompileTime>::iterator A_it{ A.begin() };
	typename std::initializer_list<std::initializer_list<scalarType>>::iterator i{};

	for (i = list.begin(); i < list.end(); ++i)
	{
		typename std::initializer_list<scalarType>::iterator j{};
		for (j = (*i).begin(); j < (*i).end(); ++j)
		{
			*A_it = *j;
			++A_it;
		}
	}
}

/// <summary>
/// Getter method for the number of rows of the matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline int Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>::rows() const
{
	return _rows;
}

/// <summary>
/// Getter method for the number of columns of the matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline int Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>::cols() const
{
	return _cols;
}

/// <summary>
/// Getter method for the size (the total number of elements) of the matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline int Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>::size() const
{
	return A.size();
}

/// <summary>
/// Coefficient accessor.
/// This routine overloads the parentheses operator ``()``. ``A(i,j)`` is used the retrieve
/// the element \f$a_{ij}\f$ belonging to the matrix \f$A\f$. This is const-version of the method,
/// that works on const Matrix objects.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="i"></param>
/// <param name="j"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline scalarType Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator()(const int i, const int j) const
{
	typename std::array<scalarType, rowsAtCompileTime* colsAtCompileTime>::const_iterator it{ A.begin() };
	it = it + (i * _cols) + j;
	if (it < A.end())
		return *it;
	else
		throw std::out_of_range("\nError accessing an element beyond matrix bounds");
}

/// <summary>
/// Coefficient accessor.
/// This routine retrieves the element in the (i,j) place of the matrix. This works on non-const Matrix
/// objects.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="i"></param>
/// <param name="j"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline scalarType& Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator()(const int i, const int j)
{
	typename std::array<scalarType, rowsAtCompileTime* colsAtCompileTime>::iterator it{ A.begin() };
	it = it + (i * _cols) + j;
	if (it < A.end())
		return *it;
	else
		throw std::out_of_range("\nError accessing an element beyond matrix bounds");
}

/// <summary>
/// Matrix addition.
/// This routine overloads the binary addition operator ``+`` and is used to add two matrices.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime> Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator+(const Matrix& m) const
{
	Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime> result{};

	if (this->rows() == m.rows() && this->cols() == m.cols())
	{
		for (int i{}; i < A.size(); ++i)
			result.A[i] = A[i] + m.A[i];
		return result;
	}
	else
	{
		throw std::logic_error("Matrices have different dimensions; therefore cannot be added!");
	}
}

/// <summary>
/// Matrix subtraction.
/// This routine overloads the binary subtraction operator ``-`` and is used to subtract two matrices.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime> Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator-(const Matrix& m) const
{
	Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime> result{};

	if (this->rows() == m.rows() && this->cols() == m.cols())
	{
		for (int i{}; i < A.size(); ++i)
			result.A[i] = A[i] - m.A[i];
		return result;
	}
	else
	{
		throw std::logic_error("Matrices have different dimensions; therefore cannot be added!");
	}
}

/// <summary>
/// Overload the ``<<`` operator to initialize a matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="x"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>& Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator<<(const scalarType x)
{
	if (currentPosition < A.end())
	{
		*currentPosition = x;
		++currentPosition;
	}
	else
	{
		throw std::logic_error("Error: Attempting to set values beyond matrix bounds!");
	}
	return *this;
}

/// <summary>
/// Overload the ``,`` operator to initialize a matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="x"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>& Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator,(const scalarType x)
{
	if (currentPosition < A.end())
	{
		*currentPosition = x;
		++currentPosition;
	}
	else
	{
		throw std::logic_error("Error: Attempting to set values beyond matrix bounds!");
	}
	return *this;
}

/// <summary>
/// Copy assignment operator.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>& Matrix<typename scalarType, rowsAtCompileTime, colsAtCompileTime>::operator=(const Matrix& right_hand_side)
{
	if (this->rows() != right_hand_side.rows() || this->cols() != right_hand_side.cols())
		throw std::logic_error("Assignment failed, matrices have different dimensions");

	if (this == &right_hand_side)
		return *this;

	this->A = right_hand_side.A;
	this->_rows = right_hand_side._rows;
	this->_cols = right_hand_side._cols;
	this->currentPosition = right_hand_side.currentPosition;
	return *this;
}


/// <summary>
/// Matrix multiplication.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="A"></param>
/// <param name="B"></param>
/// <returns></returns>
template<typename scalarType, int m, int n, int p, int q>
Matrix<scalarType, m, q> operator*(const Matrix<scalarType, m, n>& A, const Matrix<scalarType, p, q>& B)
{
	if (n != p)
		throw std::logic_error("Error multiplying the matrices; the number of cols(A) must equal the number of rows(B)!");

	Matrix<scalarType, m, q, std::array<scalarType, m* q>> result;

	for (int i{}; i < m; ++i)
	{
		for (int k{}; k < p; ++k)
		{
			scalarType sum{};
			for (int j{}; j < n; ++j)
			{
				sum += A(i, j) * B(j, k);
			}
			result(i, k) = sum;
		}
	}

	return result;
}

/// <summary>
/// Pretty print a given matrix to the console.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="os"></param>
/// <param name="m"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
std::ostream& operator<<(std::ostream& os, Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>& m)
{
	for (int i{}; i < m.rows(); ++i)
	{
		os << "Row(";
		for (int j{}; j < m.cols(); ++j)
		{
			if (j != m.cols() - 1)
				os << m(i, j) << std::setw(5);
			else
				os << m(i, j);
			if (j % 10 == 0 && j > 0)
				os << std::endl;
		}

		os << ")" << std::endl;
	}

	return os;
}

/// <summary>
/// Scalar multiplication of a matrix with a constant.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="k"></param>
/// <param name="m"></param>
/// <returns></returns>
template<typename scalarType, int m, int n>
Matrix<scalarType, m, n> operator*(const scalarType k, Matrix<scalarType, m, n>& mat)
{
	Matrix<scalarType, m, n> result{ mat };
	for (int i{}; i < m.rows(); ++i)
		for (int j{}; j < m.cols(); ++j)
			result(i, j) *= k;

	return result;
}

/// <summary>
/// Boolean comparision operator.
/// Compares if \f$A = B\f$.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
inline bool Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>::operator==(const Matrix& right_hand_side)
{
	return (this->A == right_hand_side.A);
}

/// <summary>
/// Unary plus operator.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>& operator+(const Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>& right_hand_side)
{
	return right_hand_side;
}

/// <summary>
/// Unary minus operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType, int rowsAtCompileTime, int colsAtCompileTime>
Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>& operator-(const Matrix<scalarType, rowsAtCompileTime, colsAtCompileTime>& right_hand_side)
{
	return operator*<scalarType>(-1, right_hand_side);
}

template<typename scalarType, int m, int n, int p, int q>
Matrix<scalarType, m, q>& operator*=(Matrix<scalarType, m, n>& A, const Matrix<scalarType, p, q>& B)
{
	A = A * B;
	return A;
}

template<typename scalarType, int m, int n>
Matrix<scalarType, m, n>& operator*=(const scalarType k, Matrix<scalarType, m, n>& mat)
{
	A = k * A;
	return A;
}

template <int rowsAtCompileTime, int colsAtCompileTime>
class Matrix<double, rowsAtCompileTime, colsAtCompileTime>;

template <int rowsAtCompileTime, int colsAtCompileTime>
class Matrix<float, rowsAtCompileTime, colsAtCompileTime>;

template <int rowsAtCompileTime, int colsAtCompileTime>
class Matrix<long, rowsAtCompileTime, colsAtCompileTime>;
#endif // !Matrix_H

