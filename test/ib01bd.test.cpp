//
// Created by vlasovdl on 07.10.24.
//
#include <algorithm>
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
using fd_matrix = vlasovdl::f_matrix<double>;
using fi_matrix = vlasovdl::f_matrix<int>;

class IB01BD_Test : public ::testing::Test {
protected:
  void ib01ad_calc_MOESP(int& NOBR, int& M, int& L, int& NSMPL, int& N,
    fd_matrix& R) {
    const std::string data_path {xstr(DATA_PATH)};
    fd_matrix U,Y;
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
    fd_matrix U,Y;
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

  int ib01bd_LDWORK(const char METH, const char JOB, const char JOBCK,
    const int NOBR, const int N, const int M, const int L) {
    int LDWORK, LDW1 = 0, LDW2 = 0, LDW3 = 0;
    if (METH == 'M') {
      if (JOB == 'C' or JOB == 'A' and M == 0) {
        LDW1 = std::max( 2*(L*NOBR-L)*N+2*N, (L*NOBR-L)*N+N*N+7*N );
      }
      if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
        LDW1 = std::max({
          2*(L*NOBR-L)*N+N*N+7*N,
          (L*NOBR-L)*N+N+6*M*NOBR,
          (L*NOBR-L)*N+N+std::max( L+M*NOBR, L*NOBR + std::max( 3*L*NOBR+1, M ))
        });
      }
      if (JOBCK == 'N') {
        LDW2 = 0;
      }
      if (JOBCK == 'C' or JOBCK =='K') {
        int Aw = (M == 0 or JOB == 'C') ? N+N*N : 0;
        LDW2 = std::max( {
          (L*NOBR-L)*N+Aw+2*N+std::max(5*N, (2*M+L)*NOBR+L),
          4*(M*NOBR+N)+1,
          M*NOBR+2*N+L
        });
      }
    }
    else if (METH == 'N') {
      LDW1 = L*NOBR*N+ std::max( {
        (L*NOBR-L)*N+2*N+(2*M+L)*NOBR+L,
        2*(L*NOBR-L)*N+N*N+8*N,
        4*(M*NOBR+N)+1, M*NOBR+3*N+L } );
      if (M == 0 or JOB == 'C') {
        LDW2 = 0;
      }
      if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
        LDW2 = L*NOBR*N+M*NOBR*(N+L)*(M*(N+L)+1)+
                             std::max( (N+L)^2, 4*M*(N+L)+1 );
      }
    }
    else if (METH == 'C') {
      LDW1 = std::max({
        std::max( 2*(L*NOBR-L)*N+2*N, (L*NOBR-L)*N+N*N+7*N ),
        L*NOBR*N+ std::max( {
        (L*NOBR-L)*N+2*N+(2*M+L)*NOBR+L,
        2*(L*NOBR-L)*N+N*N+8*N,
        4*(M*NOBR+N)+1, M*NOBR+3*N+L } )
      });
      if (M == 0 or JOB == 'C') {
        LDW2 = 0;
      }
      if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
        LDW2 = L*NOBR*N+M*NOBR*(N+L)*(M*(N+L)+1)+
                             std::max( (N+L)^2, 4*M*(N+L)+1 );
      }
      if (JOBCK ==  'K') {
        LDW3 = std::max(  4*N*N+2*N*L+L*L+ std::max( 3*L,N*L ),
                       14*N*N+12*N+5 );
      }
    }
    return std::max({LDW1, LDW2, LDW3});
  }

  int ib01bd_LIWORK(const char METH, const char JOB, const char JOBCK,
    const int NOBR, const int N, const int M, const int L ) {
    int LIW1 = 0, LIW2 = 0;
    if ((METH != 'N' and M == 0)  or (JOB == 'C' and JOBCK == 'N')) {
      LIW1 = N;
    }
    if (METH != 'N' and JOB == 'C' and JOBCK != 'N') {
      LIW1 = M*NOBR+N;
    }
    if (METH == 'M' and JOB != 'C'  and JOBCK == 'N') {
      LIW1 = std::max(L*NOBR, M*NOBR);
    }
    if (METH == 'M' and JOB != 'C'  and (JOBCK == 'C' or JOBCK == 'K')) {
      LIW1 = std::max(L*NOBR, M*NOBR+N);
    }
    if (METH == 'N' or (METH == 'C' and JOB  != 'C')) {
      LIW1 = std::max(M*NOBR+N,M*(N+L));
    }
    if (JOBCK ==  'K') LIW2 = N*N;
    return std::max(LIW1, LIW2);
  }

  int ib01bd_LBWORK(const char METH, const char JOB, const char JOBCK,
    const int NOBR, const int N, const int M, const int L) {
    if (JOBCK ==  'K') return 2*N;
    return 0;
  }

public:
  IB01BD_Test(){}

  void SetUp() override { }
};

TEST_F(IB01BD_Test, MOESP_calc) {
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
  ib01bd_space_(METH, JOB, JOBCK, NOBR, N, M, L, &LIWORK, &LDWORK, &LBWORK);
  ib01bd_sizes_(METH, JOB, JOBCK,N, M, L, &LDA, &LDC, &LDB, &LDD, &LDQ,
               &LDRY, &LDS, &LDK);

  // -- Создание матриц
  fd_matrix A{LDA, N}, B{LDB, M}, C{LDC, N}, D{LDD, M}, Q{LDQ, N}, RY{LDRY, L},
            S{LDS, L}, K{LDK, L};

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

TEST_F(IB01BD_Test, sizes_calculation) {
  // GTEST_SKIP();
  // -- Конфигурация расчета
  double TOL   = -1.0;

  // -- Параметры, определяемые IB01AD
  int       NOBR, N, M, L, NSMPL;
  fd_matrix R;

  // -- Вызов IB01AD
  ib01ad_calc_MOESP(NOBR, M, L, NSMPL, N, R);

  // Определяем набор значений для расчета
  std::vector<char> METH_V  {'M', 'N', 'C'};
  std::vector<char> JOB_V   {'A', 'C', 'B', 'D'};
  std::vector<char> JOBCK_V {'C', 'K', 'N'};

  for (char METH : METH_V)
    for (char JOB : JOB_V)
      for (char JOBCK : JOBCK_V) {
        // -- Расчет рабочего пространства
        int LDWORK, LIWORK, LBWORK, LDA, LDB, LDC, LDD, LDRY, LDQ, LDS, LDK;
        ib01bd_space_(METH, JOB, JOBCK, NOBR, N, M, L, &LIWORK, &LDWORK, &LBWORK);
        ib01bd_sizes_(METH, JOB, JOBCK,N, M, L, &LDA, &LDC, &LDB, &LDD, &LDQ,
                     &LDRY, &LDS, &LDK);
        // -- Создание матриц
        fd_matrix A{LDA, N}, B{LDB, M}, C{LDC, N}, D{LDD, M}, Q{LDQ, N},
                  RY{LDRY, L}, S{LDS, L}, K{LDK, L};

        // -- Создание рабочего пространства
        fd_matrix DWORK{1}; // Here we intentionally specify an incorrect value.
        fi_matrix IWORK{LIWORK}, BWORK{LBWORK};

        // -- Диагностические переменные
        int IWARN, INFO;

        // -- Вызов Fortran процедуры
        ib01bd_(&METH, &JOB, &JOBCK, &NOBR, &N, &M, &L, &NSMPL, R.data(), R.ld(),
                A.data(), A.ld(), C.data(), C.ld(), B.data(), B.ld(), D.data(),
                D.ld(), Q.data(), Q.ld(), RY.data(), RY.ld(), S.data(), S.ld(),
                K.data(), K.ld(), &TOL, IWORK.data(), DWORK.data(), DWORK.ld(),
                BWORK.data(), &IWARN, &INFO);

        EXPECT_EQ(LDWORK, static_cast<int>(DWORK(1))) <<
          "METH = " << METH << "; " <<
          "JOB = " << JOB << "; " <<
          "JOBCK = " << JOBCK << ";\t" <<
          "LDWORK = " << LDWORK << "; " <<
          "DWORK(1) = " << static_cast<int>(DWORK(1));
      }

}

