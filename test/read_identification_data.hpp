//
// Created by vlasovdl on 07.10.24.
//
#ifndef READ_IDENTIFICATION_DATA_HPP
#define READ_IDENTIFICATION_DATA_HPP

#include <string>
#include "vlasovdl/f_matrix.hpp"

/// Read data for identification subroutines
/// @param U (output) Fortran-like matrix with input channels samples
/// @param Y (output) Fortran-like matrix with output channels samples
/// @param file_name File to read
/// @return If (>0) number of samples that have been read; if (==0) - error
/// while open the file; if (<0) - line number where have been a data error
int read_identification_data(vlasovdl::fd_matrix& U, vlasovdl::fd_matrix& Y,
                             const std::string& file_name);

#endif //READ_IDENTIFICATION_DATA_HPP
