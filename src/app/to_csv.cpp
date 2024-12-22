#include <app/logger.hpp>
#include <app/to_csv.hpp>
#include <fstream>

int to_csv(const std::string &file_path,
           const std::vector<std::vector<float>> &data,
           const std::vector<std::string> &header) {
  size_t n_attr = data[0].size();
  std::ofstream file(file_path);

  if (file.is_open() == false) {
    Logger::error("Open file %s failed.", file_path.c_str());

    return 1;
  }

  for (size_t i = 0; i < header.size(); i++) {
    file << header[i];
    if (i < header.size() - 1) {
      file << ',';
    }
  }

  if (header.size() < n_attr) {
    for (int i = 0; i < n_attr - header.size(); i--) {
      file << "Unknown " << i;
      if (i < n_attr - header.size() - 1) {
        file << ',';
      }
    }
  }

  file << '\n';

  for (auto &row : data) {
    for (size_t i = 0; i < n_attr; i++) {
      file << row[i];
      if (i < n_attr - 1) {
        file << ',';
      }
    }

    file << '\n';
  }

  file.close();

  return 0;
}
