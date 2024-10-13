//
// Created by vlasovdl on 30.09.24.
//

#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <cstdio>
#include <fstream>

#include <vlasovdl/slicot_.h>
#include <vlasovdl/f_matrix.hpp>
#include "read_identification_data.hpp"
#include "ib01.tools.hpp"

#define str(s) #s
#define xstr(s) str(s)

using namespace vlasovdl;

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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {LDSV};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, N4SID_Cholesky) {
  // -- Конфигурация расчета
  METH  = 'N';    // Using N4SID algorithm
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, N4SID_FastQR) {
  // -- Конфигурация расчета
  METH  = 'N';    // Using N4SID algorithm
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, N4SID_QR) {
  // -- Конфигурация расчета
  METH  = 'N';    // Using N4SID algorithm
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {LDWORK};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
  ASSERT_EQ(INFO, 0);
}

TEST_F(IB01AD_Test, LDWORK_Calculation) {
  // GTEST_SKIP();
  // -- Конфигурация расчета
  std::vector<char> METH_V  {'M', 'N'};
  std::vector<char> ALG_V   {'C', 'F', 'Q'};
  std::vector<char> JOBD_V  {'M', 'N'};
  std::vector<char> BATCH_V {'F', 'I', 'L', 'O'};
  std::vector<char> CONCT_V {'C', 'N'};

  CTRL  = 'N';
  NOBR  = 15;
  M     = U.cols();
  L     = Y.cols();
  NSMP  = U.rows();
  RCOND = 0.0;
  TOL   = -1.0;
  for (char METH : METH_V)
    for (char ALG : ALG_V)
      for (char JOBD : JOBD_V)
        for (char BATCH : BATCH_V)
          for (char CONCT : CONCT_V) {
            int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
            ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
            ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);
            R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
            SV = fd_matrix {L*NOBR};
            IWORK = fi_matrix {LIWORK};
            DWORK = fd_matrix {1}; // Not correct DWORK
            // Вызов Fortran процедуры
            ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
                    &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N,
                    R.data(), R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(),
                    DWORK.data(), DWORK.ld(), &IWARN, &INFO);

            if (INFO == -23) {
              EXPECT_EQ(LDWORK,static_cast<int>(DWORK(1))) <<
             "LDWORK not equal: METH= " << METH <<
             ", ALG = " << ALG <<
             ", JOBD = " << JOBD <<
             ", BATCH = " << BATCH <<
             ", CONCT = " << CONCT <<
             ", INFO = " << INFO <<
             ", LDWORK = " << LDWORK <<
             ", DWORK(1) = " << static_cast<int>(DWORK(1));
            } else {
              EXPECT_EQ(INFO,0);
            }
          }
}

TEST_F(IB01AD_Test, DebugTest) {
  GTEST_SKIP();
  // -- Конфигурация расчета
  METH  = 'M';    // Using MOESP  algorithm with past inputs and outputs
  ALG   = 'F';    // Cholesky algorithm
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

  int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
  ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
  ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

  R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
  SV = fd_matrix {L*NOBR};
  IWORK = fi_matrix {LIWORK};
  DWORK = fd_matrix {1};

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
          &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
          R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
          DWORK.ld(), &IWARN, &INFO);
  ASSERT_EQ(INFO, -23);
}