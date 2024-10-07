/**
 * vlasovdl <vlasovdl @ gmail.com>
 * 05/10/2024
 **/
#ifndef VLASOVDL_SLICOTPP_IB01_HPP
#define VLASOVDL_SLICOTPP_IB01_HPP

#include <vlasovdl/f_matrix.hpp>

namespace vlasovdl { namespace slicotpp {
namespace ib01ad_p {
enum class method : char {
  MOESP     = 'M',    // MOESP algorithm with past inputs and outputs
  N4SID     = 'N'     // N4SID algorithm
};

}
struct ib01ad_parameters {
  /// The subspace identification method to be used
  /*enum class method : char {
    MOESP     = 'M',    // MOESP algorithm with past inputs and outputs
    N4SID     = 'N'     // N4SID algorithm
  } METH = method::MOESP;*/
  ib01ad_p::method METH = ib01ad_p::method::MOESP;

  /// The algorithm for computing the triangular factor R
  enum class algorithm : char {
    Cholesky  = 'C',    // Cholesky algorithm applied to the correlation matrix
                        // of the input-output data
    FastQR    = 'F',    // Fast QR algorithm
    QR        = 'Q'     // QR algorithm applied to the concatenated block Hankel
                        // matrices
  } ALG = algorithm::Cholesky;

  /// Whether the matrices B and D should later be computed using the
  /// MOESP approach
  enum class compute_input_sens : char {
    Yes       = 'M',    // The matrices B and D should later be computed using
                        // the MOESP approach
    No        = 'N'     // The matrices B and D should not be computed using
                        // the MOESP approach
  } JOBD = compute_input_sens::No;

  /// Whether sequential data processing is to be used, and, for sequential
  /// processing, whether the current data block is the first block, an
  /// intermediate block, or the last block
  enum class batch : char {
    First     = 'F',    // The first block in sequential data processing
    Inter     = 'I',    // An intermediate block in sequential data processing
    Last      = 'L',    // The last block in sequential data processing
    One       = 'O'     // One block only (non-sequential data processing).
  } BATCH = batch::First;

  /// Whether the successive data blocks in sequential data processing belong
  /// to a single experiment
  enum class is_continuation : char {
    Yes       = 'C',    // The current data block is a continuation of the
                        // previous data block and/or it will be continued by
                        // the next data block
    No        = 'N'     // There is no connection between the current data block
                        // and the previous and/or the next ones
  } CONT = is_continuation::No;

  /// Whether the user's confirmation of the system order estimate is desired.
  enum class confirmation : char {
    Yes       = 'C',    // User's confirmation
    No        = 'N'     // no confirmation
  } CTRL = confirmation::No;

  /// The tolerance to be used for estimating the rank of matrices.
  /// @note If the user sets  RCOND > 0,  the given value of  RCOND  is used as
  /// a lower bound for the reciprocal condition number;  an m-by-n matrix whose
  /// estimated condition number is less than  1/RCOND  is considered to be of
  /// full rank.  If the user sets  RCOND <= 0,  then an implicitly computed,
  /// default tolerance, defined by RCONDEF = m*n*EPS,  is used instead, where
  /// EPS is the relative machine precision (see LAPACK Library routine DLAMCH).
  /// This parameter is not used for  METH = method::MOESP.
  double RCOND = 0.0;

  /// Absolute tolerance used for determining an estimate of the system order.
  /// @note If  TOL >= 0,  the estimate is indicated by the index of the last
  /// singular value greater than or equal to  TOL.  (Singular values less than
  /// TOL are considered as zero.) When  TOL = 0, an internally computed default
  /// value,  TOL = NOBR*EPS*SV(1),  is used, where  SV(1)  is the maximal
  /// singular value, and  EPS  is the relative machine precision (see LAPACK
  /// Library routine DLAMCH).
  /// When  TOL < 0,  the estimate is indicated by the index of the singular
  /// value that has the largest logarithmic gap to its successor.
  double TOL  = -1.0;
};

/// Preprocess the input-output data for estimating the matrices of a linear
/// time-invariant dynamical system and to find an estimate of the system order.
/// The input-output data can, optionally, be processed sequentially.
/// @param U      Input-data sequence matrix
/// @param Y      Ouput-data sequence matrix
/// @param NOBR   The number of block rows, in the input and output block Hankel
///               matrices to be processed. NOBR > 0. (In the MOESP theory NOBR
///               should be larger than n, the estimated dimension of state
///               vector)
/// @param N      (output) The estimated order of the system.
/// @param R      (output or input/output) upper triangular factor R from the
///               QR factorization of the concatenated block Hankel matrices
/// @param SV     (output) The singular values used to estimate the system order.
/// @param IWORK  Integer work array
/// @param DWORK  Double float work array
/// @param IWARN  (output) Warning code
/// @param par    (input)
/// @return 
int ib01ad (const f_d_matrix& U, const f_d_matrix& Y, const int& NOBR,
            int& N, f_d_matrix& R, f_d_matrix& SV, f_i_matrix& IWORK,
            f_d_matrix& DWORK, int& IWARN, const ib01ad_parameters& par = {}); 

}} // vlasovdl::slicotpp
#endif //VLASOVDL_SLICOTPP_IB01_HPP
