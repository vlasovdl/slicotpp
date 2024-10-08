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
