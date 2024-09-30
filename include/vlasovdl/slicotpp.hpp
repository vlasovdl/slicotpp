//
// Created by vlasovdl on 30.09.24.
//

#ifndef VLASOVDL_SLICOTPP_HPP
#define VLASOVDL_SLICOTPP_HPP
#ifdef __cplusplus
extern "C" {
#endif

 extern void ib01ad_(const char* METH, const char* ALG, const char* JOBD, const char* BATCH, const char* CONCT,
                      const char* CTRL, const int* NOBR, const int* M, const int* L, const int* NSMP,
                      const double* U, const int* LDU, const double* Y, const int* LDY, int* N, double* R, int* LDR,
                      double* SV, double* RCOND, double* TOL, int* IWORK, double* DWORK, int* LDWORK, int* IWARN,
                      int* INFO);

#ifdef __cplusplus
}
#endif
#endif //VLASOVDL_SLICOTPP_HPP
