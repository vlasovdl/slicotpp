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
 * @param NOBR  Whether the user's confirmation of the system order
 * @param M     The number of system inputs.
 * @param L     The number of system outputs.
 * @param NSMP  The number of rows of matrices  U  and  Y
 * @param [out] LDR The leading dimension of the array  R
 * @param [out] LIWORK Integer workspace size
 * @param [out] LDWORK Double float workspace size
 */
void ib01ad_ws (char METH, char ALG, char JOBD, char BATCH, char CONCT,
  int NOBR, int M, int L, int NSMP, int& LDR, int& LIWORK, int& LDWORK);

#endif //IB01_TOOLS_HPP
