//
// Created by vlasovdl on 30.09.24.
//

#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <cstdio>
#include <fstream>

#include <vlasovdl/slicot.h>
#include <vlasovdl/f_matrix.hpp>
#include "read_identification_data.hpp"

#define str(s) #s
#define xstr(s) str(s)

class IB01AD_Test : public testing::Test {
protected:
  vlasovdl::f_matrix<double>  U;    // Fortran-like matrix of inputs value
  vlasovdl::f_matrix<double>  Y;    // Fortran-like matrix of outputs value
  int                         M;    // Amount of inputs channel
  int                         L;    // Amount of outputs channel
  int                      NSMP;    // Number of samples
  std::string         data_path;    // Path to data files

public:
  IB01AD_Test() : M{}, L{}, NSMP{}, data_path{xstr(DATA_PATH)} {}

  void SetUp() override {
    const int n = read_identification_data(U, Y,
                                           data_path+"/ib01ad.test.data.txt");
    ASSERT_NE(n, 0) << "Can't read data file";
    ASSERT_GT(n, 0) << "Error in " << -n << " line of data file.";
    NSMP = n;
    M = U.cols();
    L = Y.cols();
  }
};

TEST_F(IB01AD_Test, calculation_test) {
  // -- Параметры расчета
  int     NOBR  = 15;               // The number of block rows,  s,  in the input and output block Hankel matrices
                                    // to be processed
  double  RCOND = 0.0;              // The tolerance to be used for estimating the rank of matrices.
  double  TOL   = -1.0;             // Absolute tolerance used for determining an estimate of the system order.

  // -- Конфигурация расчета
  char    METH  = 'M';              // Specifies the subspace identification method to be used
  char    ALG   = 'C';              // Specifies the algorithm for computing the triangular factor R
  char    JOBD  = 'N';              // Specifies whether the matrices B and D should later be computed using
                                    // the MOESP approach
  char    BATCH = 'O';              // Specifies whether sequential data processing is to be used
  char    CONCT = 'N';              // Specifies whether the successive data blocks in sequential data processing
                                    // belong to a single experiment
  char    CTRL  = 'N';              // Specifies whether the user's confirmation of the system order
                                    // estimate is desired
  // -- Выходные данные
  int     N;        // The estimated order of the system.

  // Определяем размер для R
  int LDR = 2*(M+L)*NOBR;
  if (METH == 'M' and JOBD == 'M') LDR = std::max(LDR, 3*M*NOBR);
  vlasovdl::f_matrix<double> R{LDR, 2 * (M + L) * NOBR};

  // Определяем вектор сингулярных значений
  vlasovdl::f_matrix<double> SV{L*NOBR};

  // Определяем массив IWORK
  int LIWORK = 3;
  if (METH == 'N')     LIWORK = std::max(LIWORK, (M + L)*NOBR);
  else if (ALG == 'F') LIWORK = std::max(LIWORK, M + L);
  vlasovdl::f_matrix<int> IWORK{LIWORK};

  // Определяем массив DWORK
  int LDWORK = 5*L*NOBR;
  vlasovdl::f_matrix<double> DWORK{LDWORK};

  // -- Диагностические переменные
  int     IWARN;
  int     INFO;

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR,
          &M, &L, &NSMP, U.cdata(), U.ld(), Y.cdata(), Y.ld(),
          &N, R.data(), R.ld(), SV.data(),
          &RCOND, &TOL,
          IWORK.data(), DWORK.data(), DWORK.ld(), &IWARN, &INFO);

  EXPECT_EQ(INFO, 0);
}
