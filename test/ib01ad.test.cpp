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

using namespace vlasovdl;
using fd_matrix = vlasovdl::f_matrix<double>;
using fi_matrix = vlasovdl::f_matrix<int>;

class IB01AD_Test : public testing::Test {
protected:
  // --- Parameters
  char      METH;   // The subspace identification method to be used
  char      ALG;    // The algorithm for computing the triangular factor R
  char      JOBD;   // Whether the matrices B and D should later be computed
  char      BATCH;  // Whether sequential data processing is to be used
  char      CONCT;  // Whether the successive data blocks in sequential
  char      CTRL;   // Whether the user's confirmation of the system order
  int       NOBR;   // The number of block rows, in the input and output block
  int       M;      // The number of system inputs.
  int       L;      // The number of system outputs.
  int       NSMP;   // The number of rows of matrices  U  and  Y
  fd_matrix U;      // Fortran-like matrix of inputs value
  fd_matrix Y;      // Fortran-like matrix of outputs value
  int       N;      // The estimated order of the system.
  fd_matrix R;      // Upper triangular factor R from the QR factorization
  fd_matrix SV;     // The singular values
  double    RCOND;  // The tolerance to be used for estimating the rank
  double    TOL;    // Absolute tolerance used for estimate of the system order.
  fi_matrix IWORK;  // Integer workspace
  fd_matrix DWORK;  // Double float workspace
  int       IWARN;  // Warning indicator
  int       INFO;   // Error indicator

public:
  IB01AD_Test() : METH(0), ALG(0), JOBD(0), BATCH(0), CONCT(0), CTRL(0),
                  NOBR(0), M{}, L{}, NSMP{}, N(0), RCOND(0), TOL(0), IWARN(0),
                  INFO(0) {}

  void SetUp() override {
    const std::string data_path {xstr(DATA_PATH)};
    const int n = read_identification_data(U, Y,
                                           data_path + "/ib01ad.test.data.txt");
    ASSERT_NE(n, 0) << "Can't read data file";
    ASSERT_GT(n, 0) << "Error in " << -n << " line of data file.";
    ASSERT_EQ(U.rows(), Y.rows());
  }
};

TEST_F(IB01AD_Test, MOESP_Cholesky) {
  // -- Конфигурация расчета
  METH  = 'M';    // Using MOESP  algorithm with past inputs and outputs
  ALG   = 'C';    // Cholesky algorithm
  JOBD  = 'N';
  BATCH = 'O';    // non-sequential data processing
  CONCT = 'N';
  CTRL  = 'N';
  NOBR  = 15;
  M     = U.cols();
  L     = Y.cols();
  NSMP  = U.rows();
  RCOND = 0.0;
  TOL   = -1.0;

  // Определяем размер для R
  int LDR = 2*(M+L)*NOBR;
  if (METH == 'M' and JOBD == 'M') LDR = std::max(LDR, 3*M*NOBR);
  R = f_matrix<double> {LDR, 2 * (M + L) * NOBR};

  // Определяем вектор сингулярных значений
  SV = f_matrix<double> {L*NOBR};

  // Определяем массив IWORK
  int LIWORK = 3;
  if      (METH == 'N') LIWORK = std::max(LIWORK, (M + L)*NOBR);
  else if (ALG  == 'F') LIWORK = std::max(LIWORK, M + L);
  IWORK = f_matrix<int> {LIWORK};

  // Вызов Fortran процедуры
  DWORK = f_matrix<double>{1};
  while (true) {
    ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
            &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
            R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
            DWORK.ld(), &IWARN, &INFO);
    if (INFO == -23) {
      int ldwork = static_cast<int>(DWORK(1));
      DWORK      = f_d_matrix{ldwork};
      continue;
    }
    break;
  }
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, MOESP_FastQR) {
  // -- Конфигурация расчета
  METH  = 'M';    // Using MOESP  algorithm with past inputs and outputs
  ALG   = 'F';    // Fast QR algorithm
  JOBD  = 'N';
  BATCH = 'O';    // non-sequential data processing
  CONCT = 'N';
  CTRL  = 'N';
  NOBR  = 15;
  M     = U.cols();
  L     = Y.cols();
  NSMP  = U.rows();
  RCOND = 0.0;
  TOL   = -1.0;

  // Определяем размер для R
  int LDR = 2*(M+L)*NOBR;
  if (METH == 'M' and JOBD == 'M') LDR = std::max(LDR, 3*M*NOBR);
  R = f_matrix<double> {LDR, 2 * (M + L) * NOBR};

  // Определяем вектор сингулярных значений
  SV = f_matrix<double> {L*NOBR};

  // Определяем массив IWORK
  int LIWORK = 3;
  if      (METH == 'N') LIWORK = std::max(LIWORK, (M + L)*NOBR);
  else if (ALG  == 'F') LIWORK = std::max(LIWORK, M + L);
  IWORK = f_matrix<int> {LIWORK};

  // Вызов Fortran процедуры
  DWORK = f_matrix<double>{1};
  while (true) {
    ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
            &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
            R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
            DWORK.ld(), &IWARN, &INFO);
    if (INFO == -23) {
      int ldwork = static_cast<int>(DWORK(1));
      DWORK      = f_d_matrix{ldwork};
      continue;
    }
    break;
  }
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, MOESP_QR) {
  // -- Конфигурация расчета
  METH  = 'M';    // Using MOESP  algorithm with past inputs and outputs
  ALG   = 'Q';    // QR algorithm
  JOBD  = 'N';
  BATCH = 'O';    // non-sequential data processing
  CONCT = 'N';
  CTRL  = 'N';
  NOBR  = 15;
  M     = U.cols();
  L     = Y.cols();
  NSMP  = U.rows();
  RCOND = 0.0;
  TOL   = -1.0;

  // Определяем размер для R
  int LDR = 2*(M+L)*NOBR;
  if (METH == 'M' and JOBD == 'M') LDR = std::max(LDR, 3*M*NOBR);
  R = f_matrix<double> {LDR, 2 * (M + L) * NOBR};

  // Определяем вектор сингулярных значений
  SV = f_matrix<double> {L*NOBR};

  // Определяем массив IWORK
  int LIWORK = 3;
  if      (METH == 'N') LIWORK = std::max(LIWORK, (M + L)*NOBR);
  else if (ALG  == 'F') LIWORK = std::max(LIWORK, M + L);
  IWORK = f_matrix<int> {LIWORK};

  // Вызов Fortran процедуры
  DWORK = f_matrix<double>{1};
  while (true) {
    ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
            &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
            R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
            DWORK.ld(), &IWARN, &INFO);
    if (INFO == -23) {
      int ldwork = static_cast<int>(DWORK(1));
      DWORK      = f_d_matrix{ldwork};
      continue;
    }
    break;
  }
  ASSERT_EQ(INFO, 0);
}
