//
// Created by vlasovdl on 03.10.24.
//
#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>

#include <vlasovdl/f_matrix.hpp>
using namespace vlasovdl;
TEST(f_matrix, create_double_indefined_size) {
  f_matrix<double> m;
  EXPECT_EQ(m.rows(),0);
  EXPECT_EQ(m.cols(),0);
  EXPECT_EQ(m.is_size_defined(), false);
}

TEST(f_matrix, create_double_vector) {
  f_matrix<double> m{10};
  EXPECT_EQ(m.rows(),10);
  EXPECT_EQ(m.cols(),1);
  EXPECT_EQ(m.data_length(),10);
  EXPECT_EQ(m.is_size_defined(), true);
}

TEST(f_matrix, insert_double_to_double) {
  f_matrix<double> m{};
  m.insert_data(5.0);
  EXPECT_EQ(m.rows(),0);
  EXPECT_EQ(m.cols(),0);
  EXPECT_EQ(m.is_size_defined(), false);
  EXPECT_EQ(m.data_length(), 1);
}

TEST(f_matrix, insert_int_to_double) {
  f_matrix<double> m{};
  m.insert_data(5);
  EXPECT_EQ(m.rows(),0);
  EXPECT_EQ(m.cols(),0);
  EXPECT_EQ(m.is_size_defined(), false);
  EXPECT_EQ(m.data_length(), 1);
}

TEST(f_matrix, insert_double_vect_to_double) {
  f_matrix<double> m{};
  const std::vector<double> v{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  m.insert_data(v);
  EXPECT_EQ(m.rows(),0);
  EXPECT_EQ(m.cols(),0);
  EXPECT_EQ(m.is_size_defined(), false);
  EXPECT_EQ(m.data_length(), 9);
}

TEST(f_matrix, insert_int_vect_to_double) {
  f_matrix<double> m{};
  const std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
  m.insert_data(v);
  EXPECT_EQ(m.rows(),0);
  EXPECT_EQ(m.cols(),0);
  EXPECT_EQ(m.is_size_defined(), false);
  EXPECT_EQ(m.data_length(), 9);
}

TEST(f_matrix, make_double_vector) {
  f_matrix<double> m{10};
  EXPECT_EQ(m.rows(), 10);
  EXPECT_EQ(m.cols(), 1);
  EXPECT_EQ(m.is_size_defined(), true);
  EXPECT_EQ(m.data_length(),10);
}

TEST(f_matrix, make_and_modify_vector) {
  f_matrix<double> m{10};
  for (int i = 1; i <= 10; i++)
    m(i) = i;
  for (int i = 1; i <= 10; i++)
    EXPECT_EQ(m(i),i);
}

TEST(f_matrix, join_vector) {
  const std::vector<int> v{1, 2, 3, 4, 5};
  f_matrix<int> m{};
  m.join_column(v);
  EXPECT_EQ(m.rows(), 5);
  EXPECT_EQ(m.cols(), 1);
  m.join_column(v);
  EXPECT_EQ(m.rows(), 5);
  EXPECT_EQ(m.cols(), 2);
}

