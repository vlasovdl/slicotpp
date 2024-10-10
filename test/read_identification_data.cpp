//
// Created by vlasovdl on 07.10.24.
//
#include "read_identification_data.hpp"

#include <fstream>

using namespace vlasovdl;

int read_identification_data(fd_matrix& U, fd_matrix& Y,
                             const std::string& file_name) {
  // Open data file for reading
  std::ifstream inp_stream(file_name, std::ios_base::in);
  if (!inp_stream)
    return 0;
  // Read amount of inputs and outputs channels
  int m, l;
  inp_stream >> m >> l;
  // Read channels data
  std::vector<std::vector<double>> buffer(m + l);
  int n = 0; // Number of data samples
  while (!inp_stream.eof()) {
    for (auto i = 0; i < m + l; ++i) {
      double value;
      inp_stream >> value;
      if (inp_stream.fail()) return -(n + 2);
      buffer[i].push_back(value);
    }
    ++n;
  }
  // Copy data from buffers to matrix
  for (auto i = 0; i < m; ++i)
    U.join_column(buffer[i]);
  for (auto i = m; i < m + l; ++i)
    Y.join_column(buffer[i]);
  return n;
}
