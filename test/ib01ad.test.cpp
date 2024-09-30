//
// Created by vlasovdl on 30.09.24.
//

#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <cstdio>

#include <vlasovdl/slicotpp.hpp>
TEST(IB01AD, calculation_test) {
  // -- Параметры расчета
  int     NOBR;     // The number of block rows,  s,  in the input and output block Hankel matrices
                    // to be processed

  int     M;        // The number of system inputs
  int     L;        // The number of system outputs
  int     NSMP;     // The number of rows of matrices  U  and  Y  (number of samples,  t)
  double  RCOND;    // The tolerance to be used for estimating the rank of matrices.
  double  TOL;      // Absolute tolerance used for determining an estimate of the system order.

  // -- Конфигурация расчета
  char    METH;     // Specifies the subspace identification method to be used
  char    ALG;      // Specifies the algorithm for computing the triangular factor R
  char    JOBD;     // Specifies whether the matrices B and D should later be computed using
                    // the MOESP approach
  char    BATCH;    // Specifies whether sequential data processing is to be used
  char    CONCT;    // Specifies whether the successive data blocks in sequential data processing
                    // belong to a single experiment
  char    CTRL;     // Specifies whether the user's confirmation of the system order estimate is desired

  // -- Входные данные
  double* pU;       // Input-data sequence matrix
  int     LDU;      // The leading dimension of the array U.
  double* pY;       // Output-data sequence matrix
  int     LDY;      // The leading dimension of the array Y.

  // -- Выходные данные
  int     N;        // The estimated order of the system.
  double* pR;       //
  int     LDR;      // The leading dimension of the array R
  double* pSV;      // The singular values used to estimate the system order.

  // -- Рабочее пространство
  int*    pIWORK;   //
  int     LIWORK;   // Size of the IWORK
  double* pDWORK;   //
  int     LDWORK;   // Size of the DWORK

  // -- Диагностические переменные
  int     IWARN;
  int     INFO;

  // Открываем файл
  FILE* inp_file = std::fopen("ib01ad.test.inp", "r");
  if (!inp_file) FAIL();

  // Пропускаем первую строку
  while (std::fgetc(inp_file) != '\n');

  // Считываем параметры расчета
  std::fscanf(inp_file,"%d %d %d %d %d %lf %lf %1c %1c %1c %1c %1c %1c",
         &NOBR, &N, &M, &L, &NSMP, &RCOND, &TOL, &METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL);

  // Считываем данные для расчета
  pU = new double[NSMP];
  pY = new double[NSMP];
  for (auto i = 0; i < NSMP; ++i) std::fscanf(inp_file,"%lf", pU + i);
  for (auto i = 0; i < NSMP; ++i) std::fscanf(inp_file,"%lf", pY + i);
  LDY = LDU = NSMP;

  // Определяем и выделяем память под R
  LDR = 2*(M+L)*NOBR;
  if (METH == 'M' and JOBD == 'M') LDR = std::max(LDR, 3*M*NOBR);
  pR = new double[LDR * 2 * (M + L) * NOBR];

  // Определяем и выделяем память под SV
  pSV = new double[L*NOBR];

  // Определяем и выделяем память под IWORK
  LIWORK = 3;
  if (METH == 'N')     LIWORK = std::max(LIWORK, (M + L)*NOBR);
  else if (ALG == 'F') LIWORK = std::max(LIWORK, M + L);
  pIWORK = new int[LIWORK];

  // Определяем и выделяем память под DWORK
  LDWORK = 5*L*NOBR;
  pDWORK = new double[LDWORK];

  // Вызов Fortran процедуры
  ib01ad_(&METH, &ALG, &JOBD, &BATCH, &CONCT, &CTRL, &NOBR, &M, &L, &NSMP, pU, &LDU, pY, &LDY,
          &N, pR, &LDR, pSV, &RCOND, &TOL, pIWORK, pDWORK, &LDWORK, &IWARN, &INFO);

  EXPECT_EQ(INFO, 0);

  delete[] pU;
  delete[] pY;
  delete[] pR;
  delete[] pSV;
  delete[] pIWORK;
  delete[] pDWORK;
  std::fclose(inp_file);
}