//
// Created by vlasovdl on 07.10.24.
//
#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>

#include <vlasovdl/slicot.h>
#include <vlasovdl/f_matrix.hpp>
#include "read_identification_data.hpp"

#define str(s) #s
#define xstr(s) str(s)

class IB01BD_Test : public ::testing::Test {
protected:
  std::string                data_path; // Path to data files
  int                        N, M, L, NSMPL;
  vlasovdl::f_matrix<double> R, SV;

public:
  IB01BD_Test() : data_path{xstr(DATA_PATH)}, N(0), M(0), L(0), NSMPL(0) {}

  static void ib01ad_calculation(const vlasovdl::f_d_matrix& U,
                                 const vlasovdl::f_d_matrix& Y,
                                 vlasovdl::f_d_matrix&       R,
                                 vlasovdl::f_d_matrix&       SV, int& N) {
    // Количество входных и выходных каналов данных
    const int m = U.cols(), l = Y.cols();

    // -- Параметры расчета
    int    NOBR  = 15;
    double RCOND = 0.0;
    double TOL   = -1.0;

    // -- Конфигурация расчета
    char METH  = 'M';
    char ALG   = 'C';
    char JOBD  = 'N';
    char BATCH = 'O';
    char CONCT = 'N';
    char CTRL  = 'N';

    // -- Диагностические переменные
    int IWARN;
    int INFO;

    // Определяем размер и создаем матрицу R
    int ldr = 2 * (m + l) * NOBR;
    if (METH == 'M' && JOBD == 'M') ldr = std::max(ldr, 3 * U.cols() * NOBR);
    R = vlasovdl::f_matrix<double>{ldr, 2 * (m + l) * NOBR};

    // Определяем размер и создаем вектора сингулярных значений
    SV = vlasovdl::f_matrix<double>{l * NOBR};

    // Определяем размер и создаем массив IWORK
    int liwork = 3;
    if (METH == 'N') liwork = std::max(liwork, (m + l) * NOBR);
    else if (ALG == 'F') liwork = std::max(liwork, m + l);
    vlasovdl::f_i_matrix IWORK{liwork};

    // Тут мы пытаемся определить необходимый размер DWORK, а затем
    // произвести расчет.
    vlasovdl::f_d_matrix DWORK{1};
    while (true) {
      ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &m, &l,
              &U.rows(), U.cdata(), U.ld(), Y.cdata(), Y.ld(), &N, R.data(),
              R.ld(), SV.data(), &RCOND, &TOL, IWORK.data(), DWORK.data(),
              DWORK.ld(), &IWARN, &INFO);
      if (INFO == -23) {
        int ldwork = static_cast<int>(DWORK(1));
        DWORK      = vlasovdl::f_d_matrix{ldwork};
        continue;
      }
      break;
    }
    ASSERT_EQ(INFO, 0);
  }

  void SetUp() override {
    vlasovdl::f_matrix<double> U; // Fortran-like matrix of inputs value
    vlasovdl::f_matrix<double> Y; // Fortran-like matrix of outputs value

    const int n = read_identification_data(U, Y,
                                           data_path + "/ib01ad.test.data.txt");
    ASSERT_NE(n, 0) << "Can't read data file";
    ASSERT_GT(n, 0) << "Error in " << -n << " line of data file.";
    M     = U.cols();
    L     = Y.cols();
    NSMPL = U.rows();
    ib01ad_calculation(U, Y, R, SV, N);
  }
};

TEST_F(IB01BD_Test, calculation) {
  const char   METH  = 'M';
  const char   JOB   = 'A';
  const char   JOBCK = 'K';
  const int    NOBR  = 15;
  const double TOL   = -1.0;
  int          IWARN, INFO;

  vlasovdl::f_matrix<double> A{N, N}, B{N, M}, C{L, N}, D{L, M}, Q{N, N},
                             RY{L, L}, S{N, L}, K{N, L};

  // Расчет и выделение IWORK
  int liw1, liw2;
  if ((METH != 'N' && M == 0) || (JOB == 'C' && JOBCK == 'N'))
    liw1 = N;
  if (METH != 'N' && JOB == 'C' && JOBCK != 'N')
    liw1 = M * NOBR + N;
  if (METH == 'M' && JOB != 'C' && JOBCK == 'N')
    liw1 = std::max(L * NOBR, M * NOBR + N);
  if (METH == 'M' && JOB != 'C' && (JOBCK == 'C' || JOBCK == 'K'))
    liw1 = std::max(L * NOBR, M * NOBR + N);
  if ((METH == 'N' || (METH == 'C' && JOB != 'C')))
    liw1 = std::max(M * NOBR + N, M * (N + L));
  if (JOBCK != 'K') liw2 = 0;
  if (JOBCK == 'K') liw2 = N * N;
  vlasovdl::f_matrix<int> IWORK{std::max(liw1, liw2)};

  // Расчет и выделение BWORK
  vlasovdl::f_matrix<int> BWORK{JOBCK == 'K' ? 2 * N : 0};

  vlasovdl::f_matrix<double> DWORK{1};

  while (true) {
    ib01bd_(&METH, &JOB, &JOBCK, &NOBR, &N, &M, &L, &NSMPL, R.data(), R.ld(),
            A.data(), A.ld(), C.data(), C.ld(), B.data(), B.ld(), D.data(),
            D.ld(), Q.data(), Q.ld(), RY.data(), RY.ld(), S.data(), S.ld(),
            K.data(), K.ld(), &TOL, IWORK.data(), DWORK.data(), DWORK.ld(),
            BWORK.data(), &IWARN, &INFO);
    if (INFO == -30) {
      int ldwork = static_cast<int>(DWORK(1));
      DWORK      = vlasovdl::f_d_matrix{ldwork};
      continue;
    }
    break;
  }
  ASSERT_EQ(INFO, 0);
}
