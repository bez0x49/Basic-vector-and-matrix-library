/**
 * @file basic_vector.h
 * @brief Implements the functionally for basic_vector. It's implemented on the
 * basis of dynamic memory.
 *
 * @version 0.4
 *
 * @date 17:51 2020-10-07
 * @author bezlikiy (nevedrb@gmail.com, bezlikiy@github.com)
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once
#ifndef BASIC_VECTOR
#define BASIC_VECTOR

#include <algorithm>
#include <concepts>
#include <fstream>

/* Changes ----------------------------------------------------------
 * ===================================================================
 * * 2020-10-07 V0.1 Added static size_t ngen for counting existing vectors.
 * * 2020-10-07 V0.2 Added template for unsigned, signed, float and double.
 *   No exception for char.
 * * 2020-10-08 V0.2.1 Added getter the fields and overload [].
 * * 2020-10-09 V0.3 Fixed some bugs. Optimize algorithms for unsigned and
 *   non-unsigned types.
 * * 2020-10-09 V0.3.1 Added documentation.
 * * 2020-10-10 V0.3.2 Added opeartion /, /=. Added `STATUS::DIVIDED_ZERO` which
 *   defines when Vector divided to zero.
 * * 2020-10-10 V0.4 Added concept check for non-character type.
 */

/** TODO ----------------------------------------------------------
 * ===================================================================
 * Add an exception for defines status when execute the next operation.
 */

namespace bez {

/**
 * @enum @class STATUS
 *
 * @brief Possible errors when executing program. If they aren't, define status
 * as `GOOD_ALLOCATOR`.
 *
 * @details If an error occurs while executing the program, the program doesn't
 * end, and the status assigns a variable.
 *
 */
enum class STATUS {
  BAD_ALLOCATOR,    // Not enough memory or bad allocate
  BOUND_ARRAY,      // Beyond the array or segmentation fault
  BAD_INITIALIZED,  // Initialization with a negative number for unsigned type
  GOOD_ALLOCATOR,   // Successful allocate and initialization
  DIVIDED_ZERO      // Divide to zero
};

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T>
concept character = std::same_as<T, char> || std::same_as<T, signed char> ||
                    std::same_as<T, unsigned char> ||
                    std::same_as<T, wchar_t> || std::same_as<T, char8_t> ||
                    std::same_as<T, char16_t> || std::same_as<T, char32_t>;

template <typename T>
concept number = arithmetic<T> && !character<T>;

/**
 * @class basic_vector basic_vector.h
 *
 * @brief Functionally implements of Vector. It's implemented on allocating
 * dynamic memory for the array. If ann error occurs, determines to `_Status`.
 * @see _Status
 *
 * @ingroup Dynamic array
 *
 * @details By default `_Status` is defined as `STATUS::GOOD_ALLOCATOR`. If not
 * enough memory or it's not allocated, the error status defines as
 * `STATUS::BAD_ALLOCATOR`. If initialized by a negative number for unsigned
 * type, the error status defines as STATUS::BAD_INITIALIZED. If indexing
 * specifies as an index that goes beyonds the array, the error status defines
 * as `STATUS::BOUND_ARRAY`. Define the errro status as `STATUS::DIVIDED_ZERO`
 * which as `STATUS::BOUND_ARRAY`. Define the errro status as
 * `STATUS::DIVIDED_ZERO` when Vector divided to zero. Contains a
 * static field `ngen`, which determines the number of existing vectors.
 *
 * @exception No exception for defines status when
 * executed after indexing and after initialized vector by negative value to
 * next operation.
 *
 * @tparam T Integral or float point number
 */
template <number T>
class basic_vector {
 protected:
  // Types:
  size_t _Length;  // Length of vector
  T *_Allocator;   // Pointer to allocated memory
  mutable STATUS _Status{STATUS::GOOD_ALLOCATOR};  // Error status

 public:
  /// @brief Allocates memory for one element and initializes it to zero.
  basic_vector() : _Length(1) {
    ngen++;
    _Allocator = new T[1];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      _Allocator[0] = 0;
  }

  /**
   * @brief Allocates memory of size `length` and initializes the array to zero.
   *
   * @param length Size of vector
   */
  basic_vector(size_t length) : _Length(length) {
    ngen++;
    _Allocator = new T[length];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t i = 0; i < length; i++) _Allocator[i] = 0;
  }

  /**
   * @brief Allocates memory of size `length` and initializes the array with the
   * value `initalizedValue`. If initializedValue < 0 then inializes to 0 with
   * the status `STATUS::BAD_INITIALIZED`.
   *
   * @exception Used only for unsigned types. @see `basic_vector() requires
   * !std::is_unsigned_v<T>`
   *
   * @param length             Size of vector
   * @param initializedValue   Value to initializate
   */
  basic_vector(size_t length,
               long initializedValue) requires(std::unsigned_integral<T>)
      : _Length(length) {
    ngen++;
    if (initializedValue < 0)
      _Status = STATUS::BAD_INITIALIZED;
    else {
      _Allocator = new T[length];
      if (_Allocator == nullptr)
        _Status = STATUS::BAD_ALLOCATOR;
      else
        for (size_t i = 0; i < length; i++)
          _Allocator[i] = static_cast<T>(initializedValue);
    }
  }

  /// @exception Used only for non-unsigned type
  basic_vector(size_t length,
               T initializedValue) requires(!std::unsigned_integral<T>)
      : _Length(length) {
    ngen++;

    _Allocator = new T[length];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t i = 0; i < length; i++) _Allocator[i] = initializedValue;
  }

  /**
   * @brief Copy
   *
   * @param other Initialized vector
   */
  basic_vector(const basic_vector<T> &other) {
    ngen++;
    _Length = other.length();
    _Allocator = new T[_Length];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t i = 0; i < _Length; i++) _Allocator[i] = other._Allocator[i];
  }

  /// @brief Frees the allocated dynamic memory, if it is allocated.
  ~basic_vector() {
    if (_Allocator != nullptr) delete[] _Allocator;
    ngen--;
  }

  /**
   * @brief Overload operation `[]` like indexing of the vector.
   *
   * @param index Index of element
   * @return Element
   */
  constexpr T &operator[](const size_t index) {
    if (index >= 0 && index < _Length) return _Allocator[index];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Length - 1];
  }

  /**
   * @brief Overload operation `[]` like returns value of the element.
   *
   * @param index Index of element
   * @return Value of element
   */
  constexpr const T &operator[](const size_t index) const {
    if (index >= 0 && index < _Length) return _Allocator[index];
    _Status = STATUS::BOUND_ARRAY;
    return _Allocator[_Length - 1];
  }

  /**
   * @brief Overload operation `=`. If it is self-assignment, retuns this.
   *
   * @param other Initialized vector
   * @return Vector
   */
  basic_vector<T> &operator=(const basic_vector<T> &other) {
    if (this == &other) return *this;

    // Frees memory, if it's allocated
    if (_Allocator != nullptr) delete[] _Allocator;

    _Length = other.length();
    _Allocator = new T[_Length];
    if (_Allocator == nullptr)
      _Status = STATUS::BAD_ALLOCATOR;
    else
      for (size_t i = 0; i < _Length; i++) _Allocator[i] = other[i];
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, basic_vector<T> &obj) {
    for (size_t i = 0; i < obj.length(); i++) os << obj[i] << ' ';
    return os;
  }

  friend std::istream &operator>>(std::istream &is, basic_vector<T> &obj) {
    for (size_t i = 0; i < obj.length(); i++) is >> obj[i];
    return is;
  }

  /**
   * @brief Set the error status
   *
   * @note Maybe unused on the next versions.
   *
   * @param status Error status
   */
  [[maybe_unused]] void setStatus(STATUS status) { _Status = status; }

  /**
   * @brief Assings value to an array element.
   *
   * @exception Undefined behavior when value < 0 for unsigned type.
   *
   * @param value Value to initialize
   * @param index Index of element
   */
  void set(const T value, const size_t index = 0) { _Allocator[index] = value; }

  /**
   * @brief Get the element of vector
   *
   * @exception no except for bound index of array
   *
   * @param index Index of element
   * @return Value of element
   */
  constexpr const T &at(const size_t index) const { return _Allocator[index]; }

  /**
   * @see constexpr const T &at(const size_t index);
   *
   * @param index Index of element
   * @return Element
   */
  constexpr T &at(const size_t index) { return _Allocator[index]; }

  /**
   * @brief Get the length of vector
   *
   * @return Length of vector
   */
  constexpr size_t length() const { return _Length; }

  /**
   * @brief Calculates the sum of all elements of the vector.
   *
   * @return Sum of elements
   */
  T sum() const {
    T total = 0;
    for (size_t i = 0; i < _Length; i++) total += _Allocator[i];
    return total;
  }

  /**
   * @brief Get the status of error
   *
   * @return Error status
   */
  STATUS status() { return _Status; }

  /**
   * @brief Get the name of error
   *
   * @return Name of error status
   */
  const std::string to_string_status() {
    switch (_Status) {
      case STATUS::BAD_ALLOCATOR:
        return "BAD_ALLOCATOR";
      case STATUS::BOUND_ARRAY:
        return "BOUND_ARRAY";
      case STATUS::BAD_INITIALIZED:
        return "BAD_INITALIZED";
      case STATUS::DIVIDED_ZERO:
        return "DIVIDED_ZERO";
      default:
        return "GOOD_ALLOCATOR";
    }
  }

  /**
   * @brief Increase each element by a `k` value.
   * @details If k = 1, then returns this.
   *
   * @param other Initialized vector
   * @param k Coefficient to increase
   * @return Vector with magnified elements
   */
  friend basic_vector<T> operator*(const basic_vector<T> &other, const T k) {
    basic_vector<T> vec(other);
    if (k == 1) return vec;
    for (size_t i = 0; i < vec.length(); i++) vec[i] = other[i] * k;
    return vec;
  }

  /// @brief Perform the same operation as `*`.
  /// @see ` basic_vector<T> operator*()`
  basic_vector<T> &operator*=(const T k) {
    if (k == 1) return *this;
    for (size_t i = 0; i < _Length; i++) _Allocator[i] *= k;
    return *this;
  }

  /**
   * @brief Decrease each element by a `k` value.
   *
   * @details If k = 0, then returns this with `STATUS::DIVIDED_ZERO`.
   *
   * @param other Initialized vector
   * @param k Coefficient to decrease
   * @return Vector
   */
  friend basic_vector<T> operator/(const basic_vector<T> &other, const T k) {
    basic_vector<T> vec(other);
    if (k == 0) {
      vec.setStatus(STATUS::DIVIDED_ZERO);
      return vec;
    }

    for (size_t i = 0; i < vec.length(); i++) vec[i] = other[i] / k;
    return vec;
  }

  /// @brief Perfom the same operation as `/`.
  /// @see `basic_vector<T> operator/()`
  basic_vector<T> &operator/=(const T k) {
    if (k == 0) {
      _Status = STATUS::DIVIDED_ZERO;
      return *this;
    }
    for (size_t i = 0; i < _Length; i++) _Allocator[i] /= k;
    return *this;
  }

  /**
   * @brief Calculate the sum of vectors. If the vectors are of different
   * lengths, it returns vector of zeros with the error status
   * `STATUS::BOUND_ARRAY`.
   *
   * @param lhs First vector
   * @param rhs Last vector
   *
   * @return Sum of vectors
   */
  friend basic_vector<T> operator+(const basic_vector<T> &lhs,
                                   const basic_vector<T> &rhs) {
    basic_vector<T> vec(lhs.length());
    if (lhs.length() != rhs.length()) {
      vec._Status = STATUS::BOUND_ARRAY;
      return vec;
    }

    for (size_t i = 0; i < lhs.length(); i++) vec[i] = lhs[i] + rhs[i];

    return vec;
  }

  /**
   * @brief Perfom the same operations as `+`. IF the lengths are different, it
   * returns `this`. @see `basic_vector<T> +operator()`
   *
   * @param other Initialized vector
   * @return Sum of vectors
   */
  basic_vector<T> &operator+=(const basic_vector<T> &other) {
    if (_Length != other.length()) return *this;

    for (size_t i = 0; i < _Length; i++) _Allocator[i] += other[i];
    return *this;
  }

  /**
   * @brief Return the different of vectors. If the lengths are different, it
   * returns a vector of zeros with the error status `STATUS::BOUND_ARRAY`.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return The difference of vectors
   */
  friend basic_vector<T> operator-(const basic_vector<T> &lhs,
                                   const basic_vector<T> &rhs) {
    basic_vector<T> vec(lhs.length());
    if (lhs.length() != rhs.length()) {
      vec._Status = STATUS::BOUND_ARRAY;
      return vec;
    }

    for (size_t i = 0; i < lhs.length(); i++) vec[i] = lhs[i] - rhs[i];
    return vec;
  }

  /**
   * @brief Perfoms the same operation as `-`. If the vectors are of different
   * lengths, returns this. @see `basic_vector<T> -operator()`
   *
   * @param other Initialized vector
   * @return The difference of vectors
   */
  basic_vector<T> &operator-=(const basic_vector<T> &other) {
    if (_Length != other.length()) return *this;

    for (size_t i = 0; i < _Length; i++) _Allocator[i] -= other[i];
    return *this;
  }

  /**
   * @brief If the lengths are different, it returns false. Otherwise sorts the
   * vectors and compares each element.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return Equality of vectors
   */
  friend bool operator==(const basic_vector<T> &lhs,
                         const basic_vector<T> &rhs) {
    if (lhs.length() != rhs.length()) return false;

    basic_vector<T> t1 = lhs, t2 = rhs;

    std::sort(t1._Allocator, t1._Allocator + t1.length());
    std::sort(t2._Allocator, t2._Allocator + t2.length());

    for (size_t i = 0; i < lhs.length(); i++)
      if (t1[i] != t2[i]) return false;

    return true;
  }

  /**
   * @brief If the lengths are different, it returns true. Otherwise check for
   * equality.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return Inequality of vectors
   */
  friend bool operator!=(const basic_vector<T> &lhs,
                         const basic_vector<T> &rhs) {
    if (lhs._Length != rhs._Length) return true;

    return !(lhs == rhs);
  }

  /**
   * @brief Calculate the modulus of the vectors and compares them. If the
   * modulus are different, it returns is the modulus of last is greater than
   * the first.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return Is the last vector is bigger than first
   */
  friend bool operator<(const basic_vector<T> &lhs,
                        const basic_vector<T> &rhs) {
    return lhs.sum() < rhs.sum();
  }

  /**
   * @brief Implementation is similar to operation `<`.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return Is the first vector is bigger than last
   */
  friend bool operator>(const basic_vector<T> &lhs,
                        const basic_vector<T> &rhs) {
    return rhs < lhs;
  }

  /**
   * @brief If the last vector is smaller than the first, it returns true,
   * otherwise false.
   *
   * @param lhs First vector
   * @param rhs Last vector
   * @return Is the last vector is bigger than first or equals its.
   */
  friend bool operator<=(const basic_vector<T> &lhs,
                         const basic_vector<T> &rhs) {
    return !(lhs > rhs);
  }

  /**
   * @brief If the first vector is smaller than the last, it return true,
   * otherwise false.
   *
   * @param lhs Last vector
   * @param rhs First vector
   * @return Is the first vector is bigger than last or equals its
   */
  friend bool operator>=(const basic_vector<T> &lhs,
                         const basic_vector<T> &rhs) {
    return !(lhs < rhs);
  }

 public:
  static size_t ngen;  // Number of existing vectors
};

template <number T>
size_t basic_vector<T>::ngen = 0;
}  // namespace bez
#endif