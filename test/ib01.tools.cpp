#include <algorithm>
//
// Created by vlasovdl on 08.10.24.
//
void ib01ad_ws(char METH, char ALG, char JOBD, char BATCH, char CONCT,
               int  NOBR, int  M, int    L, int     NSMP, int& LDR, int& LIWORK,
               int& LDWORK) {
  // Расчет LDR
  if (METH == 'M' && JOBD == 'M')
    LDR = std::max(2 * (M + L) * NOBR, 3 * M * NOBR);
  else if ((METH == 'M' && JOBD == 'N') || METH == 'N')
    LDR = 2 * (M + L) * NOBR;

  int NS = NSMP - 2 * NOBR + 1;

  // Расчет LIWORK
  LIWORK = 3;
  if (METH == 'N') LIWORK = std::max(LIWORK, (M + L) * NOBR);
  else if (ALG == 'F') LIWORK = std::max(LIWORK, M + L);

  // Расчет LDWORK
  if (ALG == 'C') {
    if (BATCH == 'F' || BATCH == 'I') {
      if (CONCT == 'C') {
        LDWORK = (4 * NOBR - 2) * (M + L);
      } else if (CONCT == 'N') {
        LDWORK = 1;
      }
    }
    if (METH == 'M') {
      if (BATCH == 'L' && CONCT == 'C') {
        LDWORK = std::max((4 * NOBR - 2) * (M + L), 5 * L * NOBR);
      } else if (BATCH == 'O' || (BATCH == 'L' && CONCT == 'N')) {
        if (JOBD == 'M') {
          LDWORK = std::max((2 * M - 1) * NOBR, (M + L) * NOBR);
          LDWORK = std::max(LDWORK, 5 * L * NOBR);
        } else if (JOBD == 'N') {
          LDWORK = 5 * L * NOBR;
        }
      }
    } else if (METH == 'N' && (BATCH == 'L' || BATCH == 'O')) {
      LDWORK = 5 * (M + L) * NOBR + 1;
    }
  } else if (ALG == 'F') {
    if (BATCH != 'O' && CONCT == 'C') {
      LDWORK = (M + L) * 2 * NOBR * (M + L + 3);
    } else if ((BATCH == 'F' || BATCH == 'I') && CONCT == 'N') {
      LDWORK = (M + L) * 2 * NOBR * (M + L + 1);
    } else if ((BATCH == 'L' && CONCT == 'N') || BATCH == 'O') {
      LDWORK = (M + L) * 4 * NOBR * (M + L + 1) + (M + L) * 2 * NOBR;
    }
  } else if (ALG == 'Q') {
    if (BATCH == 'F' && LDR >= NS) {
      LDWORK = 4 * (M + L) * NOBR;
    } else if (BATCH == 'O' && LDR >= NS) {
      if (METH == 'M') LDWORK = std::max(4 * (M + L) * NOBR, 5 * L * NOBR);
      if (METH == 'N') LDWORK = 5 * (M + L) * NOBR + 1;
    } else if (((BATCH == 'F' || BATCH == 'O') && LDR < NS) ||
      ((BATCH == 'I' || BATCH == 'L') && CONCT == 'N')) {
      LDWORK = 6 * (M + L) * NOBR;
    } else if ((BATCH == 'I' || BATCH == 'L') && CONCT == 'C') {
      LDWORK = 4 * (NOBR + 1) * (M + L) * NOBR;
    }
  }
}

void ib01bd_ws (char METH, char JOB, char JOBCK, int NOBR, int N, int M, int L,
  int& LDA, int& LDB, int& LDC, int& LDD, int& LDQ, int& LDRY, int& LDS,
  int& LDK, int& LDWORK, int& LIWORK, int& LBWORK) {
  // Расчет LDA, LDC
  if (JOB == 'A' or JOB == 'C' or ((METH == 'N' or METH == 'C') and (JOB == 'B'
    or JOB == 'D'))) {
    LDA = N; LDC = L;
  } else {
    LDA = 1; LDC = 1;
  }

  // Расчет LDB
  if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
    LDB = N;
  } else if (M == 0 or  JOB == 'C') {
    LDB = 1;
  }

  // Расчет LDD
  if (M > 0 and (JOB == 'A' or JOB == 'D')) {
    LDD = L;
  } else if (M == 0 or (JOB == 'C' or JOB == 'B')) {
    LDD = 1;
  }

  // Расчет LDQ, LDRY, LDS, LDK
  if (JOBCK == 'C' or JOBCK == 'K') {
    LDQ = N; LDRY = L; LDS = N;
  } else if (JOBCK == 'N') {
    LDQ = 1; LDRY = 1; LDS = 1;
  }

  // Расчет LDK
  if (JOBCK == 'K') {
    LDK = N;
  } else if (JOBCK == 'N' or JOBCK == 'C') {
    LDK = 1;
  }

  // Расчет LDWORK
  int LDW1 = 0, LDW2 = 0, LDW3 = 0;
  if (METH == 'M') {
    if (JOB == 'C' or (JOB == 'A' and M == 0)) {
      LDW1 = std::max(2 * (L * NOBR - L) * N + 2 * N,
                      (L * NOBR - L) * N + N * N + 7 * N);
    } else if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
      LDW1 = std::max({
        2 * (L * NOBR - L) * N + N * N + 7 * N,
        (L * NOBR - L) * N + N + 6 * M * NOBR,
        (L * NOBR - L) * N + N + std::max({
          L + M * NOBR,
          L * NOBR + std::max(3 * L * NOBR + 1, M)
        })
      });
    }
    if (JOBCK == 'N') {
      LDW2 = 0;
    } else if (JOBCK == 'C' or JOBCK == 'K') {
      int Aw = (M == 0 or JOB == 'C') ? N + N * N : 0;
      LDW2   = L * NOBR * N + std::max({
        (L * NOBR - L) * N + Aw + 2 * N + std::max(
          5 * N, (2 * M + L) * NOBR + L),
        4 * (M * NOBR + N) + 1,
        M * NOBR + 2 * N + L
      });
    }
  } else if (METH == 'N') {
    LDW1 = L * NOBR * N + std::max({
      (L * NOBR - L) * N + 2 * N + (2 * M + L) * NOBR + L,
      2 * (L * NOBR - L) * N + N * N + 8 * N,
      N + 4 * (M * NOBR + N) + 1,
      M * NOBR + 3 * N + L
    });
    if (M == 0 or JOB == 'C') {
      LDW2 = 0;
    } else if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
      LDW2 = L * NOBR * N + M * NOBR * (N + L) * (M * (N + L) + 1) + std::max(
        (N + L) ^ 2, 4 * M * (N + L) + 1);
    }
  } else if (METH == 'C') {
    LDW1 = std::max({
      std::max(2 * (L * NOBR - L) * N + 2 * N,
               (L * NOBR - L) * N + N * N + 7 * N),
      L * NOBR * N + std::max({
        (L * NOBR - L) * N + 2 * N + (2 * M + L) * NOBR + L,
        2 * (L * NOBR - L) * N + N * N + 8 * N,
        N + 4 * (M * NOBR + N) + 1,
        M * NOBR + 3 * N + L
      })
    });
    if (M == 0 or JOB == 'C') {
      LDW2 = 0;
    } else if (M > 0 and (JOB == 'A' or JOB == 'B' or JOB == 'D')) {
      LDW2 = L * NOBR * N + M * NOBR * (N + L) * (M * (N + L) + 1) + std::max(
        (N + L) ^ 2, 4 * M * (N + L) + 1);
    }
    if (JOBCK != 'K') {
      LDW3 = 0;
    } else {
      LDW3 = std::max(4 * N * N + 2 * N * L + L * L + std::max(3 * L, N * L),
                      14 * N * N + 12 * N + 5);
    }
  }
  LDWORK = std::max ({LDW1, LDW2, LDW3});

  // Расчет LIWORK
  int LIW1 = 0, LIW2 = 0;
  if (METH != 'N' and (M == 0 or (JOB == 'C' and JOBCK == 'N'))) {
    LIW1 = N;
  } else if (METH != 'N' and JOB == 'C' and JOBCK != 'N') {
    LIW1 = M * NOBR + N;
  } else if (METH == 'M' and JOB != 'C' and JOBCK == 'N') {
    LIW1 = std::max(L * NOBR, M * NOBR);
  } else if (METH == 'M' and JOB != 'C' and (JOBCK == 'C' or JOBCK == 'K')) {
    LIW1 = std::max(L * NOBR, M * NOBR + N);
  } else if (METH == 'N' or (METH == 'C' and JOB != 'C')) {
    LIW1 = std::max(M * NOBR + N, M * (N + L));
  }
  if (JOBCK != 'K') {
    LIW2 = 0;
  } else {
    LIW2 = N * N;
  }
  LIWORK = std::max(LIW1, LIW2);

  // Расчет LBWORK
  if (JOBCK == 'K') {
    LBWORK = 2 * N;
  } else {
    LBWORK = 0;
  }
}
