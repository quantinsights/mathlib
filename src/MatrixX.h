#pragma once
#include <array>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <iomanip>
#include <initializer_list>
#include <algorithm>
#include "slice.h"
#include <cassert>

template <typename T>
class MatrixX;

using MatrixXi = MatrixX<int>;
using MatrixXd = MatrixX<double>;
using MatrixXf = MatrixX<float>;

using VectorXf = MatrixXf;
using VectorXd = MatrixXd;
using VectorXi = MatrixXi;

/// <summary>
/// MatrixRowSlice is a proxy object that represents a row of a matrix without being the row itself. 
/// It holds a reference to a MatrixX object, a slice and a row/col index. We are then free to implement
/// how this proxy behaves, by having its operations inspect and manipulate the matrix it was created from.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template <class scalarType>
class MatrixRowSlice 
{
private:
	MatrixX<scalarType>& _matrix_ref;
	slice _matrix_slice;
	int _row;
public:
	MatrixRowSlice() = default;
	MatrixRowSlice(MatrixX<scalarType>& m_ref, slice s, int r) : _matrix_ref{ m_ref }, _matrix_slice{ s }, _row{ r } {}
	
	//Overload operators
	MatrixRowSlice operator=(const MatrixRowSlice matrixSlice) const;
	MatrixRowSlice<scalarType> operator=(const MatrixX<scalarType>& rowVector);

	slice getMatrixSlice() const;
	int getRow() const;
	MatrixX<scalarType>& getMatrixRef() const;
};

/// <summary>
/// MatrixColSlice is a proxy object that represents a column of a matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
template <class scalarType>
class MatrixColSlice 
{
private:
	MatrixX<scalarType>& _matrix_ref;
	slice _matrix_slice;
	int _col;
public:
	MatrixColSlice() = default;
	MatrixColSlice(MatrixX<scalarType>& m_ref, slice s, int c) : _matrix_ref{ m_ref }, _matrix_slice{ s }, _col{ c } {}

	//Overload operators
	MatrixColSlice operator=(const MatrixColSlice matrixSlice) const;
	MatrixColSlice<scalarType> operator=(const MatrixX<scalarType>& colVector);

	slice getMatrixSlice() const;
	int getCol() const;
	MatrixX<scalarType>& getMatrixRef() const;
};

/// <summary>
/// Return a ref to the matrix object from which this row vector was created.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<class scalarType>
MatrixX<scalarType>& MatrixRowSlice<scalarType>::getMatrixRef() const
{
	return _matrix_ref;
}

/// <summary>
/// Return a ref to the matrix object from which this col vector was created.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<class scalarType>
MatrixX<scalarType>& MatrixColSlice<scalarType>::getMatrixRef() const
{
	return _matrix_ref;
}


template<class scalarType>
slice MatrixRowSlice<scalarType>::getMatrixSlice() const
{
	return _matrix_slice;
}

template<class scalarType>
slice MatrixColSlice<scalarType>::getMatrixSlice() const
{
	return _matrix_slice;
}

template<class scalarType>
int MatrixRowSlice<scalarType>::getRow() const
{
	return _row;
}

template<class scalarType>
int MatrixColSlice<scalarType>::getCol() const
{
	return _col;
}

/// <summary>
/// Overloaded assignment operator. This allows you to do `m.row(1) = m.row(2);`
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="s"></param>
/// <returns></returns>
template <class scalarType>
MatrixRowSlice<scalarType> MatrixRowSlice<scalarType>::operator=(const MatrixRowSlice s) const
{
	assert(_matrix_slice.length == s.getMatrixSlice().getLength());
	MatrixX<scalarType>& otherMatrix = s.getMatrixRef();
	slice otherSlice{ s.getMatrixSlice() };

	for (int j{}; i < _matrix_slice.length; ++j)
		_matrix_ref(_row, _matrix_slice(j)) = otherMatrix(s.getRow(),s.getMatrixSlice(j));
	return *this;
}

/// <summary>
/// Overloaded assignment operator. This allows you to do `m.row(1) = VectorXd {{1,2,3}};`
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="s"></param>
/// <returns></returns>
template<class scalarType>
MatrixRowSlice<scalarType> MatrixRowSlice<scalarType>::operator=(const MatrixX<scalarType>& rowVector)
{
	assert(rowVector.rows() == 1);
	assert(rowVector.cols() == _matrix_slice.getLength());
	for (int j{}; j < rowVector.cols(); ++j)
		_matrix_ref(_row, _matrix_slice(j)) = rowVector(0, j);
	
	return *this;
}

/// <summary>
/// Overloaded assignment operator. This allows you to do `m.col(1) = m.col(2);`
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="s"></param>
/// <returns></returns>
template <class scalarType>
MatrixColSlice<scalarType> MatrixColSlice<scalarType>::operator=(const MatrixColSlice s) const
{
	assert(_matrix_slice.length == s.getMatrixSlice().getLength());
	MatrixX<scalarType>& rhs = s.getMatrixRef();
	slice rhsSlice{ s.getMatrixSlice() };

	for (int i{}; i < _matrix_slice.length; ++i)
		_matrix_ref(rhsSlice(i), _col) = rhs(s.getMatrixSlice(i),s.getCol() );
	return *this;
}

/// <summary>
/// Overloaded assignment operator. This allows you to assign a user-supplied column vector
/// to a given column of the matrix m, something like `m.col(1) = VectorXd {{1},{2},{3}};`
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="s"></param>
/// <returns></returns>
template<class scalarType>
MatrixColSlice<scalarType> MatrixColSlice<scalarType>::operator=(const MatrixX<scalarType>& colVector)
{
	assert(colVector.cols() == 1);
	assert(colVector.rows() == _matrix_slice.length);
	for (int i{}; i < colVector.rows(); ++i)
		_matrix_ref(_matrix_slice(i),_col ) = colVector(i, 0);

	return *this;
}


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
	MatrixX(int n);
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
	MatrixX& operator=(const MatrixRowSlice<scalarType>& rhs);
	MatrixX& operator=(const MatrixColSlice<scalarType>& rhs);
	bool operator==(const MatrixX& right_hand_side);
	MatrixX& operator+=(const MatrixX& m);
	MatrixX& operator-=(const MatrixX& m);

	//Submatrices and sub-vectors
	MatrixRowSlice<scalarType> MatrixX<scalarType>::row(int i);
	MatrixColSlice<scalarType> MatrixX<scalarType>::col(int j);

	MatrixX<scalarType> transpose() const;
};

// ===========================================================================================
//                                   Global Operators
// -------------------------------------------------------------------------------------------
template<typename scalarType>
MatrixX<scalarType> operator*(const MatrixX<scalarType>& A, const MatrixX<scalarType>& B);

template<typename scalarType>
MatrixX<scalarType> operator*(const scalarType k, MatrixX<scalarType>& m);

template<typename scalarType>
MatrixX<scalarType>& operator*=(MatrixX<scalarType>& A, const MatrixX<scalarType>& B);

template<typename scalarType>
MatrixX<scalarType>& operator*=(const scalarType k, MatrixX<scalarType>& m);

// ===========================================================================================

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
/// Construct a column vector \f$v \in \mathbf{R}^{n}\f$.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="m"></param>
/// <param name="n"></param>
template<typename scalarType>
MatrixX<typename scalarType>::MatrixX(int n) : MatrixX(n, 1)
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
	if (i * cols() + j < A.size())
		return A[i * cols() + j];
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
	if (i * cols() + j < A.size())
		return A[i * cols() + j];
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
		for (int i{}; i < A.size(); ++i)
		{
			result.A[i] = A[i] + m.A[i];
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
		for (int i{}; i < A.size(); ++i)
		{
			result.A[i] = A[i] - m.A[i];
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
MatrixX<typename scalarType>& MatrixX<typename scalarType>::operator=(const MatrixX& rhs)
{
	if (this->rows() != rhs.rows() || this->cols() != rhs.cols())
		throw std::logic_error("Assignment failed, matrices have different dimensions");

	if (this == &rhs)
		return *this;

	this->A = rhs.A;
	this->_rows = rhs._rows;
	this->_cols = rhs._cols;
	this->currentPosition = rhs.currentPosition;
	return *this;
}

template<typename scalarType>
MatrixX<scalarType>& MatrixX<scalarType>::operator=(const MatrixRowSlice<scalarType>& rhs)
{
	slice s{ rhs.getMatrixSlice() };
	MatrixX<scalarType>& mat = rhs.getMatrixRef();
	this->_rows = 1;
	this->_cols = s.getLength();
	this->_size = this->_cols;
	this->A = std::vector<scalarType>(_cols);
	for (int j{}; j < s.getLength(); ++j)
	{
		A[j] = mat(rhs.getRow(), s(j));
	}
	this->currentPosition = mat.currentPosition;
	return *this;
}

template<typename scalarType>
MatrixX<scalarType>& MatrixX<scalarType>::operator=(const MatrixColSlice<scalarType>& rhs)
{
	slice s{ rhs.getMatrixSlice() };
	MatrixX<scalarType>& mat = rhs.getMatrixRef();
	this->_rows =  s.getLength();
	this->_size = this->rows();
	this->_cols = 1;
	this->A = std::vector<scalarType>(_rows);
	for (int i{}; i < s.getLength(); ++i)
	{
		A[i] = mat(s(i), rhs.getCol());
	}
	this->currentPosition = mat.currentPosition;
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

/// <summary>
/// Get the ith row vector.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="i"></param>
/// <returns></returns>
template<class scalarType>
MatrixRowSlice<scalarType> MatrixX<scalarType>::row(int i)
{
	slice s{ 0,cols(),1 };
	return MatrixRowSlice<scalarType>{*this,s,i};
}

/// <summary>
/// Get the jth column vector.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <param name="j"></param>
/// <returns></returns>
template<class scalarType>
MatrixColSlice<scalarType> MatrixX<scalarType>::col(int j)
{
	slice s{ 0,rows(),1 };
	return MatrixRowSlice<scalarType>{*this, s, j};
}

/// <summary>
/// Transpose the matrix.
/// </summary>
/// <typeparam name="scalarType"></typeparam>
/// <returns></returns>
template<class scalarType>
MatrixX<scalarType> MatrixX<scalarType>::transpose() const
{
	MatrixX<scalarType> result{ cols(),rows() };
	for (int i{}; i < rows(); ++i)
		for (int j{}; j < cols(); ++j)
			result(j, i) = (*this) (i, j);
	
	result._rows = cols();
	result._cols = rows();
	return result;
}
