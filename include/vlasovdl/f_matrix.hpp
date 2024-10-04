//
// Created by vlasovdl on 03.10.24.
//

#ifndef VLASOVDL_F_MATRIX_HPP
#define VLASOVDL_F_MATRIX_HPP

#include <cassert>
#include <vector>
#include <type_traits>


namespace vlasovdl {
namespace f_matrix_traits {
template <typename my_vt, typename some_vt>
using is_suitable_value_type = typename std::is_convertible<some_vt, my_vt>::type;

template <typename my_vt, typename some_vt>
constexpr bool is_suitable_value = is_suitable_value_type<my_vt, some_vt>::value &&
                                   std::is_scalar<some_vt>::value;

template <typename my_vt, typename itr>
constexpr bool is_suitable_iterator = is_suitable_value_type<my_vt, typename std::iterator_traits<itr>::value_type>::value;

}

template<typename, typename = void>
class f_matrix {};

template <class value_type>
class f_matrix<value_type, std::enable_if_t<std::is_arithmetic<value_type>::value>> {
  std::vector<value_type> vector_;    // Storage vector
  int                     rows_;      // Amount of rows. Leading dimension
  int                     cols_;      // Amount of columns.
public:
  /// Create fortran matrix.
  /// @param rows How many rows (default = 0)
  /// @param cols How many columns (default = 1)
  /// @note Without argument (or when rows == 0) it will be creating an indefinite size matrix. One
  /// should be filing data by insert_data() method, or by join_column() method. When rows > 0 and
  /// cols is absent it will be a vector. When size is defined (matrix or vector) it filled
  /// default value of type value_t.
  explicit f_matrix(const int rows = 0, const int cols = 1)
    : vector_(rows * cols), rows_(rows), cols_(rows > 0 ? cols : 0) {
    assert (cols > 0);
  }

  /// Get rows of matrix
  const int& rows() const {
    return rows_;
  }

  /// Get cols of matrix
  const int& cols() const {
    return cols_;
  }

  bool is_size_defined() const {
    return rows_ >  0;
  }

  /// Get reference to matrix element
  /// @param row Row in [1 .. rows()]
  /// @param col Columns in [1 .. cols()]
  /// @return Reference to element M(row,col)
  value_type& operator()(const int row, const int col) {
    // When rows_ and cols_ are zero, it's mean that the matrix is not a definitely size. In that
    // case impossible to get data from there. Further, indexes of element should be in range of matrix
    // sizes.
    assert(is_size_defined());
    assert(col <= cols_ and row <= rows_);
    return vector_[ rows_*(row-1) + (col-1) ];
  }

  /// Get reference to vector element
  /// @param row Row in [1 ... rows()]
  /// @note It must be a vector: cols() == 1.
  value_type& operator()(const int row) {
    assert(is_size_defined());
    assert(cols_ == 1 and row <= rows_);
    return vector_[row-1];
  }

  /// Insert data of convertible type to matrix
  /// @tparam arithmetic_value_type Type of inserted data
  /// @param data Data to be inserted
  /// @note Inserting data does not set or change the matrix size. The inserted data just placed
  /// in storage. The matrix size must then be set by resize() method.
  template <class arithmetic_value_type>
  std::enable_if_t<f_matrix_traits::is_suitable_value<value_type, arithmetic_value_type>>
  insert_data(const arithmetic_value_type& data) {
    assert(!is_size_defined());
    vector_.push_back(data);
  }

  /// Insert data of convertible type from vector to matrix
  /// @tparam arithmetic_value_type Type of inserted data
  /// @param data Vector with data to be inserted
  /// @note Inserting data does not set or change the matrix size. The inserted data just placed
  /// in storage. The matrix size must then be set by resize() method.
  /// todo: This method should be refactored for using with arbitrary suitable container
  template <typename arithmetic_value_type>
  std::enable_if_t<f_matrix_traits::is_suitable_value<value_type, arithmetic_value_type>>
  insert_data(const std::vector<arithmetic_value_type>& data) {
    assert(!is_size_defined());
    vector_.insert(std::end(vector_), std::begin(data), std::end(data));
  }

  /// Insert data of convertible type from range to matrix
  /// @param first First element iterator
  /// @param last  Before last element iterator
  /// @note Inserting data does not set or change the matrix size. The inserted data just placed
  /// in storage. The matrix size must then be set by resize() method.
  template<class InputIt>
  std::enable_if_t<f_matrix_traits::is_suitable_iterator<value_type, InputIt>>
  insert_data(InputIt first, InputIt last) {
    assert(!is_size_defined());
    vector_.insert(end(vector_), first, last);
  }

  /// Insert data of convertible type from range to matrix. With iterator decrement.
  /// @tparam InputIt Iterator type
  /// @param first First element iterator
  /// @param last Before last element iterator
  /// @param dec Iterator decrement
  /// @note Inserting data does not set or change the matrix size. The inserted data just placed
  /// in storage. The matrix size must then be set by resize() method.
  template<class InputIt>
  std::enable_if_t<f_matrix_traits::is_suitable_iterator<value_type, InputIt>>
  insert_data(InputIt first, InputIt last, typename std::iterator_traits<InputIt>::difference_type dec) {
    assert(!is_size_defined());
    while (first != last) {
      vector_.push_back(*first);
      first += dec;
    }
  }

  /// Get length of data
  int data_length() {
    return vector_.size();
  }

  /// Set vector size
  void set_size() {
    assert(!is_size_defined());
    rows_ = data_length();
    cols_ = rows_ > 0 ? 1 : 0;
  }

  /// Set matrix size
  void set_size(const int rows) {
    assert(!is_size_defined());
    assert(rows > 0);
    const int n = data_length(); // amount of data
    assert (n % rows == 0);
    if (n % rows == 0) {
      rows_ = rows;
      cols_ = n / rows_;
    }
  }

  /// Join a column to matrix at right.
  /// @param column Vector of suitable data type and suitable size.
  /// @note In case when the matrix is undefined size join a column will make a vector of same
  /// size as vector being merged. If matrix already has size, then it is possible to join only
  /// vector with size equal to rows().
  template<typename T>
  std::enable_if_t<f_matrix_traits::is_suitable_value<value_type,T>>
  join_column(const std::vector<T>& column) {
    if (!is_size_defined()) {
      insert_data(column);
      set_size();
    } else if ( rows_ == column.size()) {
      vector_.insert(std::end(vector_),std::cbegin(column), std::cend(column));
      ++cols_;
    }
  }

  /// Get pointer to lead dimension
  /// @return Pointer to constant integer
  const int* ld() {
    return &rows_;
  }

  /// Get pointer to data
  /// @return Pinter to matrix data
  value_type* data() {
    return vector_.data();
  }


  /// Get constant pointer to matrix data
  /// @return
  const value_type* cdata() {
    return vector_.data();
  }
};
} // namespace vlasovdl

#endif //VLASOVDL_F_MATRIX_HPP
