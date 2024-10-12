//
// Created by vlasovdl on 08.10.24.

#ifndef IB01_TOOLS_HPP
#define IB01_TOOLS_HPP

/**
 * Calculate sizes of workspace for IB01AD subroutine.
 * @param METH  The subspace identification method to be used
 * @param ALG   The algorithm for computing the triangular factor R
 * @param JOBD  Whether the matrices B and D should later be computed
 * @param BATCH Whether sequential data processing is to be used
 * @param CONCT Whether the successive data blocks in sequential
 * @param NOBR  The number of block rows
 * @param M     The number of system inputs.
 * @param L     The number of system outputs.
 * @param NSMP  The number of rows of matrices  U  and  Y
 * @param [out] LDR The leading dimension of the array  R
 * @param [out] LIWORK Integer workspace size
 * @param [out] LDWORK Double float workspace size
 */
void ib01ad_ws (char METH, char ALG, char JOBD, char BATCH, char CONCT,
  int NOBR, int M, int L, int NSMP, int& LDR, int& LIWORK, int& LDWORK);

/**
 * Calculate sizes of workspace for IB01BD subroutine.
 * @param METH  The subspace identification method to be used
 * @param JOB   Which matrices should be computed
 * @param JOBCK Computing the covariance matrices and the Kalman gain matrix
 * @param NOBR  The number of block rows
 * @param N     The order of the system
 * @param M     The number of system inputs
 * @param L     The number of system outputs
 * @param [out] LDA    The leading dimension of A
 * @param [out] LDB    The leading dimension of B
 * @param [out] LDC    The leading dimension of C
 * @param [out] LDD    The leading dimension of D
 * @param [out] LDQ    The leading dimension of Q
 * @param [out] LDRY   The leading dimension of RY
 * @param [out] LDS    The leading dimension of S
 * @param [out] LDK    The leading dimension of K
 * @param [out] LDWORK Double float workspace size
 * @param [out] LIWORK Integer workspace size
 * @param [out] LBWORK Logical workspace size
 */
void ib01bd_ws(char METH, char JOB, char JOBCK, int NOBR, int N, int M, int L,
  int& LDA, int& LDB, int& LDC, int& LDD, int& LDQ, int& LDRY, int& LDS,
  int& LDK, int& LDWORK, int& LIWORK, int& LBWORK);

#ifdef __cplusplus
extern "C" {
#endif

extern void ib01ad_space_(char METH, char ALG, char JOBD, char BATCH,
  char CONCT, int NOBR, int M, int L, int NSMP, int *LIWORK, int *LDWORK);

extern void ib01ad_sizes_(char METH, char JOBD, int NOBR, int M, int L,
  int* LDR, int* LDSV);

extern void ib01bd_space_(char METH, char JOB, char JOBCK, int NOBR, int N,
  int M, int L, int *LIWORK, int *LDWORK, int *LBWORK);

extern void ib01bd_sizes_(char METH, char JOB, char JOBCK, int N,
  int M, int L, int *LDA, int *LDC, int *LDB, int *LDD, int *LDQ, int *LDRY,
  int *LDS, int *LDK);

extern void ib01cd_space_(char JOBX0, char COMUSE, char JOB, int N, int M,
  int L, int NSMP, int* LIWORK, int* LDWORK);

extern void ib01cd_minsmp_(char JOBX0, char COMUSE, char JOB, int N, int M,
    int L, int* MINSMP);

#ifdef __cplusplus
}
#endif
#endif //IB01_TOOLS_HPP
