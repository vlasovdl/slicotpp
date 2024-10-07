//
// Created by vlasovdl on 05.10.24.
//
#include "vlasovdl/slicotpp/ib01.hpp"
#include "vlasovdl/slicot.h"

namespace vlasovdl { namespace slicotpp {

int ib01ad(const f_d_matrix& U, const f_d_matrix& Y, const int& NOBR, int& N,
  f_d_matrix& R, f_d_matrix& SV, f_i_matrix& IWORK, f_d_matrix& DWORK,
  int& IWARN, const ib01ad_parameters& par) {
  // Invoke Fortran subroutine
  int info = 0;
  ib01ad_(reinterpret_cast<const char*>(&par.METH),
          reinterpret_cast<const char*>(&par.ALG),
          reinterpret_cast<const char*>(&par.JOBD),
          reinterpret_cast<const char*>(&par.BATCH),
          reinterpret_cast<const char*>(&par.CONT),
          reinterpret_cast<const char*>(&par.CTRL),
          &NOBR, &U.cols(), &Y.cols(), &U.rows(), U.cdata(), &U.rows(),
          Y.cdata(), &Y.rows(), &N, R.data(), R.ld(), SV.data(), &par.RCOND,
          &par.TOL, IWORK.data(), DWORK.data(),DWORK.ld(), &IWARN, &info);
  // return info;
  ib01ad_parameters pars;
  {
    using namespace ib01ad_p;
    pars.METH = method::MOESP;
    pars.ALG  = ib01ad_parameters::algorithm::Cholesky;
  }
  pars.METH = ib01ad_p::method::MOESP;
}

}}
