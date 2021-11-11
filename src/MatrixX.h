#pragma once
#include <array>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <iomanip>
#include <initializer_list>

template <typename T>
class MatrixX;

using MatrixXi = MatrixX<int>;
using MatrixXd = MatrixX<double>;
using MatrixXf = MatrixX<float>;

/// <summary>
/// ``MatrixX`` is a templated class that implements dynamic matrices.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template <typename scalarType>
class MatrixX
{
private:
	std::vector<scalarType> A;
	int _rows;
	int _cols;
	int _size;
	typename std::vector<scalarType>::iterator currentPosition;
public:
	MatrixX();
	MatrixX(int m, int n);
	MatrixX(const MatrixX& m);
	MatrixX(std::initializer_list<std::initializer_list<scalarType>>);

	std::vector<scalarType> getRawData() const;
	int rows() const;
	int cols() const;
	int size() const;

	//Overloaded operators
	scalarType operator()(const int i, const int j) const;
	scalarType& operator()(const int i, const int j);
	MatrixX operator+(const MatrixX& m) const;
	MatrixX operator-(const MatrixX& m) const;
	MatrixX& operator<<(const scalarType x);
	MatrixX& operator,(const scalarType x);
	MatrixX& operator=(const MatrixX& right_hand_side);
	bool operator==(const MatrixX& right_hand_side);
	MatrixX& operator+=(const MatrixX& m);
	MatrixX& operator-=(const MatrixX& m);

};

// Multiplication operator
template<typename scalarType>
MatrixX<scalarType> operator*(const MatrixX<scalarType>& A, const MatrixX<scalarType>& B);

template<typename scalarType>
MatrixX<scalarType> operator*(const scalarType k, MatrixX<scalarType>& m);

template<typename scalarType>
MatrixX<scalarType>& operator*=(MatrixX<scalarType>& A, const MatrixX<scalarType>& B);

template<typename scalarType>
MatrixX<scalarType>& operator*=(const scalarType k, MatrixX<scalarType>& m);

// Sub-matrices and sub-vectors
template<class scalarType>
MatrixX<scalarType> row(const MatrixX<scalarType>& m, int i);

template<class scalarType>
MatrixX<scalarType> col(const MatrixX<scalarType>& m, int j);

/// <summary>
/// Default constructor.
/// No memory allocations are performed here.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template<typename scalarType>
MatrixX<typename scalarType>::MatrixX() :_rows{ 0 }, _cols{ 0 }, _size{ 0 }
{
	currentPosition = A.begin();
}

/// <summary>
/// Copy constructor.
/// Creates a deep copy of the MatrixX object passed.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template<typename scalarType>
MatrixX<typename scalarType>::MatrixX<typename scalarType>(const MatrixX& m) : A{ m.A }, _rows{ m.rows() }, _cols{ m.cols() }, currentPosition{ m.currentPosition }, _size{ m.size() }
{
}

/// <summary>
/// Construct a matrix \f$A \in \mathbf{R}^{m \times n}\f$.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <param name="n"></param>
template<typename scalarType>
MatrixX<typename scalarType>::MatrixX(int m, int n) : _rows{ m }, _cols{ n }, _size{ m * n }, A(m * n)
{
	currentPosition = A.begin();
}

/// <summary>
/// Allocates and initializes a matrix using the curly brace initializer list.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="list"></param>
template<typename scalarType>
MatrixX<typename scalarType>::MatrixX(std::initializer_list<std::initializer_list<scalarType>> list) :MatrixX<scalarType>{}	//Delegate to the default constructor to set up the initial array
{
	typename std::initializer_list<std::initializer_list<scalarType>>::iterator i{};
	_rows = list.size();
	for (i = list.begin(); i < list.end(); ++i)
	{
		typename std::initializer_list<scalarType>::iterator j{};
		for (j = (*i).begin(); j < (*i).end(); ++j)
		{
			if (j == (*i).begin())
				_cols = (*i).size();
			A.push_back(*j);
		}
	}
	_size = _rows * _cols;
	currentPosition = A.begin();
}

/// <summary>
/// Getter method for the number of rows of a matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType>
int MatrixX<scalarType>::rows() const
{
	return _rows;
}

/// <summary>
/// Getter method for the number of columns of a matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType>
int MatrixX<scalarType>::cols() const
{
	return _cols;
}

/// <summary>
/// Getter method for the size (the total number of elements) of the matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<typename scalarType>
int MatrixX<scalarType>::size() const
{
	return A.size();
}

template<typename scalarType>
std::vector<scalarType> MatrixX<scalarType>::getRawData() const
{
	return A;
}

/// <summary>
/// Coefficient accessor.
/// This routine overloads the parentheses operator ``()``. ``A(i,j)`` is used the retrieve
/// the element \f$a_{ij}\f$ belonging to the matrix \f$A\f$. This is const-version of the method,
/// that works on const MatrixX objects.
/// </summary>
template<typename scalarType>
scalarType MatrixX<typename scalarType>::operator()(const int i, const int j) const
{
	typename std::vector<scalarType>::const_iterator it{ A.begin() };
	it = it + (i * _cols) + j;
	if (it < A.end())
		return *it;
	else
		throw std::out_of_range("\nError accessing an element beyond matrix bounds");
}

/// <summary>
/// Coefficient accessor.
/// This routine retrieves the element in the (i,j) place of the matrix. This works on non-const MatrixX
/// objects.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="i"></param>
/// <param name="j"></param>
/// <returns></returns>
template<typename scalarType>
scalarType& MatrixX<typename scalarType>::operator()(const int i, const int j)
{
	typename std::vector<scalarType>::iterator it{ A.begin() };
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
template<typename scalarType>
MatrixX<typename scalarType> MatrixX<typename scalarType>::operator+(const MatrixX& m) const
{
	if (this->rows() == m.rows() && this->cols() == m.cols())
	{
		MatrixX<scalarType> result{ m.rows(),m.cols() };
		typename std::vector<scalarType>::const_iterator it1{ A.begin() };
		typename std::vector<scalarType>::const_iterator it2{ m.A.begin() };
		typename std::vector<scalarType>::iterator resultIter{ result.A.begin() };
		while (it1 < A.end() && it2 < m.A.end())
		{
			*resultIter = *it1 + *it2;
			++it1; ++it2; ++resultIter;
		}
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
template<typename scalarType>
MatrixX<typename scalarType> MatrixX<typename scalarType>::operator-(const MatrixX& m) const
{
	if (this->rows() == m.rows() && this->cols() == m.cols())
	{
		MatrixX<scalarType> result{ m.rows(),m.cols() };
		typename std::vector<scalarType>::const_iterator it1{ A.begin() };
		typename std::vector<scalarType>::const_iterator it2{ m.A.begin() };
		typename std::vector<scalarType>::iterator resultIter{ result.A.begin() };
		while (it1 < A.end() && it2 < m.A.end())
		{
			*resultIter = *it1 - *it2;
			++it1; ++it2; ++resultIter;
		}
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
template<typename scalarType>
MatrixX<typename scalarType>& MatrixX<typename scalarType>::operator<<(const scalarType x)
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
template<typename scalarType>
MatrixX<typename scalarType>& MatrixX<typename scalarType>::operator,(const scalarType x)
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
template<typename scalarType>
MatrixX<typename scalarType>& MatrixX<typename scalarType>::operator=(const MatrixX& right_hand_side)
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
template<typename scalarType>
MatrixX<scalarType> operator*(const MatrixX<scalarType>& A, const MatrixX<scalarType>& B)
{
	if (A.cols() != B.rows())
		throw std::logic_error("Error multiplying the matrices; the number of cols(A) must equal the number of rows(B)!");

	MatrixX<scalarType> result{ A.rows(), B.cols() };

	for (int i{}; i < A.rows(); ++i)
	{
		for (int k{}; k < B.cols(); ++k)
		{
			scalarType sum{};
			for (int j{}; j < A.cols(); ++j)
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
template<typename scalarType>
std::ostream& operator<<(std::ostream& os, MatrixX<scalarType>& m)
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
template<typename scalarType>
MatrixX<scalarType> operator*(const scalarType k, MatrixX<scalarType>& m)
{
	MatrixX<scalarType> result{ m };
	for (int i{}; i < m.rows(); ++i)
	{
		for (int j{}; j < m.cols(); ++j)
		{
			result(i, j) = m(i,j) * k;
		}
	}

	return result;
}

/// <summary>
/// Boolean comparision operator.
/// Compares if \f$A = B\f$.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType>
bool MatrixX<scalarType>::operator==(const MatrixX& right_hand_side)
{
	return (this->A == right_hand_side.A);
}

/// <summary>
/// Unary plus operator.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType>
MatrixX<scalarType>& operator+(MatrixX<scalarType>& right_hand_side)
{
	return right_hand_side;
}

/// <summary>
/// Unary minus operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="right_hand_side"></param>
/// <returns></returns>
template<typename scalarType>
MatrixX<scalarType> operator-(MatrixX<scalarType>& right_hand_side)
{
	return operator*<scalarType>(-1, right_hand_side);
}

/// <summary>
/// Addition assignment operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <returns></returns>
template<class scalarType>
MatrixX<scalarType>& MatrixX<scalarType>::operator+=(const MatrixX<scalarType>& m)
{
	(*this) = (*this) + m;
	return (*this);
}

/// <summary>
/// Subtraction assignment operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <returns></returns>
template<class scalarType>
MatrixX<scalarType>& MatrixX<scalarType>::operator-=(const MatrixX<scalarType>& m)
{
	(*this) = (*this) - m;
	return (*this);
}

/// <summary>
/// Multiplication assignment operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="A"></param>
/// <param name="B"></param>
/// <returns></returns>
template<typename scalarType>
MatrixX<scalarType>& operator*=(MatrixX<scalarType>& A, const MatrixX<scalarType>& B)
{
	A = A * B;
	return A;
}

/// <summary>
/// Scalar multiplication assignment operator
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="k"></param>
/// <param name="A"></param>
/// <returns></returns>
template<typename scalarType>
MatrixX<scalarType>& operator*=(const scalarType k, MatrixX<scalarType>& A)
{
	A = k * A;
	return A;
}

template<class scalarType>
MatrixX<scalarType> row(const MatrixX<scalarType>& m, int i)
{
	MatrixX<scalarType> result{ 1, m.cols() };

	for (int j{}; j < m.cols(); ++j)
	{
		result(0, j) = m(i, j);
	}
	return result;
}

template<class scalarType>
MatrixX<scalarType> col(const MatrixX<scalarType>& m, int j)
{
	MatrixX<scalarType> result{ m.rows(), 1 };

	for (int i{}; i < m.rows(); ++i)
	{
		result(i, 0) = m(i, j);
	}
	return result;
}