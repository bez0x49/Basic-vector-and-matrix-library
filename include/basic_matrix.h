/**
 * @file Matrix.h
 * @brief Implements the functionally for Matrix.
 *
 * @version 0.1.1
 *
 * @date 15:00 2020-10-09
 * @author bezlikiy (nevedrb@gmail.com, bezlikiy@github.com)
 *
 * @copyright Copyright (c) 2020
 *
 */

/* Changes ----------------------------------------------------------
 * ===================================================================
 * * 2020-10-09 V0.1 Implement the Matrix.
 * * 2020-10-09 V0.1.1 Added documentation.
 * * 2020-10-10 V0.2 Aded overload of `+=`, `-=`, `/`, `/=`, `*=`, `>=`, `<=`.
 * * 2020-10-10 V0.2.1 Added defines _Status with STATUS::DIVIDED_ZERO
 */

#pragma once
#ifndef BASIC_MATRIX
#define BASIC_MATRIX

#include "basic_vector.h"

namespace bez {

/**
 * @class Matrix Matrix.h
 *
 * @brief Functionally implements of Matrix. It's implemented on allocating
 * dynamic memory for the array of basic_vector. If an error occurs, determines
 * to
 * `_Status`. @see _Status @see basic_vector
 *
 * @ingroup Dynamic array of basic_vector
 *
 * @details Defined the error status such as basic_vector.
 *
 * @exception Undefined behavior for char. No exception for overload _Status.
 *
 * @tparam T Integral or float point number
 */
template <typename T>
class basic_matrix {
 private:
  size_t _Rows;                 // Rows of Matrix
  size_t _Columns;              // Columns of Matrix
  basic_vector<T> *_Allocator;  // Allocator memory for basic_vector

 public:
  /// @brief Initialize Matrix 5x5 to zero
  basic_matrix() : _Rows(5), _Columns(5) {
    _Allocator = new basic_vector<T>[5];
    if (_Allocator == nullptr)
      STATUS::BAD_ALLOCATOR;
    else
      for (size_t row = 0; row < 5; row++) _Allocator[row] = basic_vector<T>(5);
  }

  /**
   * @brief Initialize Matrix `size` x `size` to zero.
   *
   * @param size Size of matrix
   */
  basic_matrix(size_t size) : _Rows(size), _Columns(size) {
    _Allocator = new basic_vector<T>[size];
    if (_Allocator == nullptr)
      STATUS::BAD_ALLOCATOR;
    else
      for (size_t row = 0; row < _Rows; row++)
        _Allocator[row] = basic_vector<T>(size);
  }

  /**
   * @brief Initialize matrix `rows` x `columns` to `value`.
   *
   * @param rows    Rows of matrix
   * @param columns Columsn of matrix
   * @param value   Value to initialize
   */
  basic_matrix(size_t rows, size_t columns, T value = 0)
      : _Rows(rows), _Columns(columns) {
    _Allocator = new basic_vector<T>[_Rows];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t row = 0; row < rows; row++)
        _Allocator[row] = basic_vector<T>(columns, value);
  }

  /// @brief Copy constructor.
  /// @param other Initialized Matrix
  basic_matrix(const basic_matrix<T> &other) {
    _Status = other._Status;
    _Rows = other.rows();
    _Allocator = new basic_vector<T>[_Rows];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t row = 0; row < _Rows; row++)
        _Allocator[row] = other._Allocator[row];
  }

  /// @brief Free the allocated dynamic memory, if it is allocated.
  ~basic_matrix() {
    if (_Allocator != nullptr) delete[] _Allocator;
  }

  /**
   * @brief Overload operation `=`. If it is self-assignment, retuns this.
   *
   * @param other Initialized vector
   * @return Matrix
   */
  basic_matrix<T> &operator=(const basic_matrix<T> &other) {
    if (this == &other) return *this;

    _Status = other._Status;

    if (_Allocator != nullptr) delete[] _Allocator;

    _Rows = other._Rows;
    _Columns = other._Columns;
    _Allocator = new basic_vector<T>[_Rows];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t row = 0; row < _Rows; row++)
        _Allocator[row] = other._Allocator[row];

    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const basic_matrix<T> &obj) {
    for (size_t i = 0; i < obj.rows(); i++)
      os << obj._Allocator[i] << std::endl;
    return os;
  }

  /**
   * @brief Set the value in Matrix[row][column].
   *
   * @param row     Index of row
   * @param column  Index of column
   * @param value   Value to initialize
   */
  void set(const size_t row, const size_t column, const T value = 0) {
    _Allocator[row][column] = value;
  }

  /**
   * @brief Get the element
   *
   * @param row     Index of row
   * @param column  Index of column
   * @return        r-value of element
   */
  constexpr const T &at(const size_t row, const size_t column) const {
    if (row >= 0 && row < _Rows) return _Allocator[row][column];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Rows - 1][_Columns - 1];
  }

  /**
   * @brief Set the element like r-value.
   *
   * @param row     Index of row
   * @param column  Index of column
   * @return l-value of element
   */
  constexpr T &at(const size_t row, const size_t column) {
    if (row >= 0 && row < _Rows) return _Allocator[row][column];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Rows - 1][_Columns - 1];
  }

  /**
   * @brrief Overload operation `()` like matrix access [][].
   *
   * @exception Implement in basic_stream @see basic_stream
   *
   * @param row     Index of row
   * @param column  Index of column
   * @return l-value of element
   */
  constexpr T &operator()(const size_t row, const size_t column) {
    if (row >= 0 && row < _Rows) return _Allocator[row][column];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Rows - 1][_Columns - 1];
  }

  /**
   * @brrief Overload operation `()` like matrix access [][].
   *
   * @exception Implement in basic_stream @see basic_stream
   *
   * @param row     Index of row
   * @param column  Index of column
   * @return r-value of element
   */
  constexpr const T &operator()(const size_t row, const size_t column) const {
    if (row >= 0 && row < _Rows) return _Allocator[row][column];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Rows - 1][_Columns - 1];
  }

  /// @brief Overload `+`.
  friend basic_matrix<T> operator+(const basic_matrix<T> &lhs,
                                   const basic_matrix<T> &rhs) {
    basic_matrix<T> matrix(lhs);
    if ((lhs._Rows != rhs._Rows) && (lhs._Columns != rhs._Columns)) {
      matrix._Status = STATUS::BOUND_ARRAY;
      return matrix;
    }

    for (size_t i = 0; i < lhs._Rows; i++)
      matrix._Allocator[i] = (lhs._Allocator[i] + rhs._Allocator[i]);

    return matrix;
  }

  /// @brief Overload `-`.
  friend basic_matrix<T> operator-(const basic_matrix<T> &lhs,
                                   const basic_matrix<T> &rhs) {
    basic_matrix<T> matrix(lhs);
    if ((lhs._Rows != rhs._Rows) && (lhs._Columns != rhs._Columns)) {
      matrix._Status = STATUS::BOUND_ARRAY;
      return matrix;
    }

    for (size_t i = 0; i < lhs._Rows; i++)
      matrix._Allocator[i] = (lhs._Allocator[i] - rhs._Allocator[i]);

    return matrix;
  }

  /**
   * @brief Return the product of Matrix. The size must be matching size.
   * Otherwise return Matrix with one element of zero.
   *
   * @param lhs First matrix
   * @param rhs First matrix
   * @return Product of Matrix
   */
  friend basic_matrix<T> operator*(const basic_matrix<T> &lhs,
                                   const basic_matrix<T> &rhs) {
    if (lhs._Rows != rhs._Columns && lhs._Columns != rhs._Rows)
      return basic_matrix<T>{1, 1};

    size_t rows, columns;
    if (lhs._Rows >= rhs._Columns) {
      rows = lhs._Rows;
      columns = rhs._Columns;
    } else {
      rows = lhs._Columns;
      columns = rhs._Rows;
    }

    basic_matrix<T> matrix(rows, columns);

    for (size_t row = 0; row < rows; row++)
      for (size_t col = 0; col < columns; col++)
        for (size_t inner = 0; inner < rows; inner++)
          matrix(row, col) += lhs(row, inner) * rhs(inner, col);

    return matrix;
  }

  /**
   * @brief Overload `/`.
   *
   * @param other Matrix
   * @param k
   * @return Matrix
   */
  friend basic_matrix<T> operator/(const basic_matrix<T> &other, const T k) {
    basic_matrix<T> matrix(other);

    if (k == 0) {
      matrix._Status = STATUS::DIVIDED_ZERO;
      return matrix;
    }

    for (size_t i = 0; i < other.size(); i++) {
      matrix._Allocator[i] = (other._Allocator[i] / k);
    }

    return matrix;
  }

  /// @brief Overload `+=`.
  basic_matrix<T> &operator+=(const basic_matrix<T> &other) {
    if (_Rows != other.rows() && _Columns != other.columns()) return *this;

    for (size_t i = 0; i < _Rows; i++) _Allocator[i] += other._Allocator[i];
    return *this;
  }

  /// @brief Overload `-=`.
  basic_matrix<T> &operator-=(const basic_matrix<T> &other) {
    if (_Rows != other.rows() && _Columns != other.columns()) return *this;

    for (size_t i = 0; i < _Rows; i++) _Allocator[i] -= other._Allocator[i];
    return *this;
  }

  basic_matrix<T> &operator/=(const T k) {
    if (k == 0) {
      _Status = STATUS::DIVIDED_ZERO;
      return *this;
    }

    for (size_t i = 0; i < _Rows; i++) {
      _Allocator[i] /= k;
    }

    return *this;
  }

  /// @brief Overload `*=`.
  basic_matrix<T> &operator*=(const basic_matrix<T> &other) {
    if (_Rows != other._Columns && _Columns != other._Rows) return *this;

    size_t rows, columns;
    if (_Rows >= other._Columns) {
      rows = _Rows;
      columns = other._Columns;
    } else {
      rows = _Columns;
      columns = other._Rows;
    }

    basic_matrix<T> matrix(rows, columns);

    for (size_t row = 0; row < rows; row++)
      for (size_t col = 0; col < columns; col++)
        for (size_t inner = 0; inner < rows; inner++)
          matrix(row, col) += this->operator()(row, inner) * other(inner, col);

    *this = matrix;
    return *this;
  }

  /// @brief Get the error status
  STATUS status() const { return _Status; }

  /// @brief Get the name of error status
  const std::string to_string_status() { _Allocator->to_string_status(); }

  /**
   * @brief Get the rows.
   *
   * @return Num of rows
   */
  constexpr size_t rows() const { return _Rows; }

  /**
   * @brief Get the columns.
   *
   * @return Num of columns
   */
  constexpr size_t columns() const { return _Columns; }

  /// @brief Implement like in basic_vector @see basic_vector
  friend bool operator==(const basic_matrix<T> &lhs,
                         const basic_matrix<T> &rhs) {
    if ((lhs._Rows != rhs._Rows) && (lhs._Columns != rhs._Columns))
      return false;

    for (size_t row = 0; row < lhs.rows(); row++)
      if (lhs._Allocator[row] != rhs._Allocator[row]) return false;

    return true;
  }

  friend bool operator!=(const basic_matrix<T> &lhs,
                         const basic_matrix<T> &rhs) {
    return !(lhs == rhs);
  }

  /**
   * @brief Calculate the modulus of each vectors and compares the values.
   * Implements like in @see basic_vector
   *
   * @param lhs First Matrix
   * @param rhs Last Matrix
   * @return Is the last vector is bigger than first
   */
  friend bool operator<(const basic_matrix<T> &lhs,
                        const basic_matrix<T> &rhs) {
    T sum1 = 0;
    T sum2 = 0;

    for (size_t row1 = 0; row1 < lhs.rows(); row1++)
      sum1 += lhs._Allocator[row1].sum();
    for (size_t row2 = 0; row2 < rhs.rows(); row2++)
      sum2 += rhs._Allocator[row2].sum();

    return sum1 < sum2;
  }

  friend bool operator>(const basic_matrix<T> &lhs,
                        const basic_matrix<T> &rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const basic_matrix<T> &lhs,
                         const basic_matrix<T> &rhs) {
    return !(lhs > rhs);
  }

  friend bool operator>=(const basic_matrix<T> &lhs,
                         const basic_matrix<T> &rhs) {
    return !(lhs < rhs);
  }

 private:
  mutable STATUS _Status{STATUS::GOOD_ALLOCATOR};  // Error status
};
}  // namespace bez
#endif