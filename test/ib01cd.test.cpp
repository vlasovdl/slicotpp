//
// Created by vlasovdl on 12.10.24.
//
#include <gmock/internal/gmock-internal-utils.h>
#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>

#include <vlasovdl/slicot.h>
#include <vlasovdl/f_matrix.hpp>
#include "read_identification_data.hpp"
#include "ib01.tools.hpp"

#define str(s) #s
#define xstr(s) str(s)

using namespace vlasovdl;

class IB01CD_Test : public ::testing::Test {
protected:
  // Матрицы входных данных используются при последующих вычислениях. Поэтому
  // лучше сохранить их как элемент класса.
  fd_matrix U, Y;
  void ib01ad_calc_MOESP(int& NOBR, int& M, int& L, int& NSMPL, int& N,
    fd_matrix& R) {
    const std::string data_path {xstr(DATA_PATH)};
    const int n = read_identification_data(U, Y,
                                           data_path + "/ib01ad.test.data.txt");
    ASSERT_NE(n, 0) << "Can't read data file";
    ASSERT_GT(n, 0) << "Error in " << -n << " line of data file.";
    ASSERT_EQ(U.rows(), Y.rows());

    // -- Конфигурация расчета
    char   METH   = 'M'; // Using MOESP  algorithm with past inputs and outputs
    char   ALG    = 'C'; // Cholesky algorithm
    char   JOBD   = 'M';
    char   BATCH  = 'O'; // non-sequential data processing
    char   CONCT  = 'N';
    char   CTRL   = 'N';
    NOBR   = 15;
    M      = U.cols();
    L      = Y.cols();
    int    NSMP   = NSMPL = U.rows();
    double RCOND  = 0.0;
    double TOL    = -1.0;


    int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
    ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
    ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

    R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
    fd_matrix SV{L*NOBR};
    fi_matrix IWORK {LIWORK};
    fd_matrix DWORK {LDWORK};
    int IWARN, INFO;

    // Вызов Fortran процедуры
    ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
            &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
            R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
            DWORK.ld(), &IWARN, &INFO);
    ASSERT_EQ(INFO, 0);
  }
  void ib01ad_calc_N4SID(int& NOBR, int& M, int& L, int& NSMPL, int& N,
    fd_matrix& R) {
    const std::string data_path {xstr(DATA_PATH)};
    const int n = read_identification_data(U, Y,
                                           data_path + "/ib01ad.test.data.txt");
    ASSERT_NE(n, 0) << "Can't read data file";
    ASSERT_GT(n, 0) << "Error in " << -n << " line of data file.";
    ASSERT_EQ(U.rows(), Y.rows());

    // -- Конфигурация расчета
    char   METH   = 'N'; // Using N4SID  algorithm with past inputs and outputs
    char   ALG    = 'C'; // Cholesky algorithm
    char   JOBD   = 'N';
    char   BATCH  = 'O'; // non-sequential data processing
    char   CONCT  = 'N';
    char   CTRL   = 'N';
    NOBR   = 15;
    M      = U.cols();
    L      = Y.cols();
    int    NSMP   = NSMPL = U.rows();
    double RCOND  = 0.0;
    double TOL    = -1.0;


    int LDR=0, LDSV = 0, LIWORK=0, LDWORK=0;
    ib01ad_space_(METH,ALG,JOBD,BATCH,CONCT,NOBR,M,L,NSMP,&LIWORK,&LDWORK);
    ib01ad_sizes_(METH,ALG,NOBR,M,L,&LDR,&LDSV);

    R  = fd_matrix {LDR, 2 * (M + L) * NOBR};
    fd_matrix SV{L*NOBR};
    fi_matrix IWORK {LIWORK};
    fd_matrix DWORK {LDWORK};
    int IWARN, INFO;

    // Вызов Fortran процедуры
    ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L,
            &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
            R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
            DWORK.ld(), &IWARN, &INFO);
    ASSERT_EQ(INFO, 0);
  }
  void ib01bd_calc(fd_matrix& A, fd_matrix& B, fd_matrix& C, fd_matrix& D) {
    // -- Конфигурация расчета
    char   METH  = 'M';
    char   JOB   = 'A';
    char   JOBCK = 'N';
    double TOL   = -1.0;

    // -- Параметры, определяемые IB01AD
    int       NOBR, N, M, L, NSMPL;
    fd_matrix R;

    // -- Вызов IB01AD
    ib01ad_calc_MOESP(NOBR, M, L, NSMPL, N, R);

    // -- Расчет рабочего пространства
    int LDWORK, LIWORK, LBWORK, LDA, LDB, LDC, LDD, LDRY, LDQ, LDS, LDK ;
    ib01bd_space_(METH, JOB, JOBCK, NOBR, N, M, L,&LIWORK, &LDWORK, &LBWORK);
    ib01bd_sizes_(METH,JOB,JOBCK,N,M,L,&LDA,&LDC, &LDB, &LDD, &LDQ,
                 &LDRY, &LDS, &LDK);

    // -- Создание матриц
    A = fd_matrix{LDA, N}; B = fd_matrix{LDB, M}; C = fd_matrix{LDC, N};
    D = fd_matrix{LDD, M};

    fd_matrix Q{LDQ, N}, RY{LDRY, L}, S{LDS, L}, K{LDK, L};

    // -- Создание рабочего пространства
    fd_matrix DWORK{LDWORK};
    fi_matrix IWORK{LIWORK}, BWORK{LBWORK};

    // -- Диагностические переменные
    int IWARN, INFO;

    // -- Вызов Fortran процедуры
    ib01bd_(&METH, &JOB, &JOBCK, &NOBR, &N, &M, &L, &NSMPL, R.data(), R.ld(),
            A.data(), A.ld(), C.data(), C.ld(), B.data(), B.ld(), D.data(),
            D.ld(), Q.data(), Q.ld(), RY.data(), RY.ld(), S.data(), S.ld(),
            K.data(), K.ld(), &TOL, IWORK.data(), DWORK.data(), DWORK.ld(),
            BWORK.data(), &IWARN, &INFO);

    ASSERT_EQ(INFO, 0);
  }
};

TEST_F(IB01CD_Test, calculation) {
  // GTEST_SKIP();
  // -- Конфигурация расчета
  double TOL    = -1.0;

  // -- Параметры, определяемые IB01AD и IB01CD
  fd_matrix A, B, C, D;
  ib01bd_calc(A, B, C, D);
  int N    = A.rows();
  int M    = U.cols();
  int L    = Y.cols();
  int NSMP = U.rows();

  std::vector<char> JOBX0_V  {'X', 'N'};
  std::vector<char> COMUSE_V {'C', 'U', 'N'};
  std::vector<char> JOB_V    {'B', 'D'};

  for (char JOBX0 : JOBX0_V) for (char COMUSE : COMUSE_V) for (char JOB : JOB_V) {
    // -- Определяем и создаем рабочее пространство
    int LIWORK, LDWORK;
    ib01cd_space_(JOBX0, COMUSE, JOB, N, M, L, NSMP, &LIWORK, &LDWORK);
    fi_matrix IWORK{LIWORK};
    fd_matrix DWORK{LDWORK};

    // -- Определяем выходные данные
    int LDX0, LDV;
    ib01cd_sizes_(JOBX0, COMUSE, JOB, N, M, L, NSMP, &LDX0, &LDV);
    fd_matrix X0 {LDX0};
    fd_matrix  V {LDV, N};
    int IWARN, INFO;

    // -- Вызываем fortran процедуру
    ib01cd_(&JOBX0, &COMUSE, &JOB, &N, &M, &L, &NSMP, A.cdata(), A.ld(), B.data(),
      B.ld(), C.cdata(), C.ld(), D.data(), D.ld(), U.data(), U.ld(), Y.cdata(),
      Y.ld(), X0.data(), V.data(), V.ld(), &TOL, IWORK.data(), DWORK.data(),
      DWORK.ld(), &IWARN, &INFO);

    ASSERT_EQ(INFO, 0);
  }
}

TEST_F(IB01CD_Test, ib01bd_calculation) {
  fd_matrix A, B, C, D;
  ib01bd_calc(A, B, C, D);
}