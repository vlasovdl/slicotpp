//
// Created by vlasovdl on 30.09.24.
//

#ifndef VLASOVDL_SLICOT_H
#define VLASOVDL_SLICOT_H
#ifdef __cplusplus
extern "C" {
#endif
/**
* Preprocess the input-output data for estimating the matrices of a linear
* time-invariant dynamical system and to find an estimate of the system order.
* The input-output data can, optionally, be processed sequentially.
**/
extern void ib01ad_(const char* METH, const char* ALG, const char* JOBD,
  const char* BATCH, const char* CONCT, const char* CTRL, const int* NOBR,
  const int* M, const int* L, const int* NSMP, const double* U, const int* LDU,
  const double* Y, const int* LDY, int* N, double* R, const int* LDR, double* SV,
  const double* RCOND, const double* TOL, int* IWORK, double* DWORK,
  const int* LDWORK, int* IWARN, int* INFO);

extern void ib01ad_space_(const char *METH, const char *ALG, const char *JOBD,
  const char *BATCH, const char *CONCT, const int *NOBR, const int *M,
  const int *L, const int *NSMP, int *LIWORK, int *LDWORK);

extern void ib01ad_sizes_(const char *METH, const char *JOBD, const int *NOBR,
  const int *M, const int *L, int* LDR, int* LDSV);

/**
* Estimate the system matrices A, C, B, and D, the noise covariance matrices Q,
* Ry, and S, and the Kalman gain matrix K of a linear time-invariant state space
* model, using the processed triangular factor R of the concatenated block
* Hankel matrices, provided by SLICOT Library routine IB01AD.
**/
extern void ib01bd_(const char* METH, const char* JOB, const char* JOBCK,
  const int* NOBR, const int* N, const int* M, const int* L, const int* NSMPL,
  double* R, const int* LDR, double* A, const int* LDA, double* C,
  const int* LDC, double* B, const int* LDB, double* D, const int* LDD,
  double* Q, const int* LDQ, double* RY, const int* LDRY, double* S,
  const int* LDS, double* K, const int* LDK, const double* TOL, int* IWORK,
  double* DWORK, const int* LDWORK, int* BWORK, int* IWARN, int* INFO);

extern void ib01bd_space_(const char *METH, const char *JOB, const char *JOBCK,
  const int *NOBR, const int *N, const int *M, const int *L, int *LIWORK,
  int *LDWORK, int *LBWORK);

extern void ib01bd_sizes_(const char *METH, const char *JOB, const char *JOBCK,
  const int *N, const int *M, const int *L, int *LDA, int *LDC, int *LDB,
  int *LDD, int *LDQ, int *LDRY, int *LDS, int *LDK);

/**
* Estimate the initial state and, optionally, the system matrices B  and  D
* of a linear time-invariant (LTI) discrete-time system, given the system
* matrices  (A,B,C,D),  or (when  B  and  D  are estimated) only the matrix
* pair  (A,C),  and the input and output trajectories of the system.
**/
extern void ib01cd_(const char* JOBX0, const char* COMUSE, const char* JOB,
  const int* N, const int* M, const int* L, const int* NSMP, const double* A,
  const int* LDA, double* B, const int* LDB, const double* C, const int* LDC,
  double* D, const int* LDD, double* U, const int* LDU, const double* Y,
  const int* LDY, double* X0, double* V, const int* LDV, const double* TOL,
  int* IWORK, double* DWORK, const int* LDWORK, int* IWARN, int* INFO);

extern void ib01cd_space_(const char *JOBX0, const char *COMUSE,
  const char *JOB, const int *N, const int *M, const int *L, const int *NSMP,
  int* LIWORK, int* LDWORK);

extern void ib01cd_sizes_(const char *JOBX0, const char *COMUSE,
  const char *JOB, const int *N, const int *M, const int *L, const int *NSMP,
  int* LDX0, int* LDV);

#ifdef __cplusplus
}
#endif
#endif //VLASOVDL_SLICOT_H
