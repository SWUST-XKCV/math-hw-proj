#pragma once

#include <string>
#include <vector>

int to_csv(const std::string &file_path,
           const std::vector<std::vector<float>> &data,
           const std::vector<std::string> &header);
