#pragma once

#include <string>
#include <vector>

std::vector<std::vector<float>>
impute_missing_by_mean(const std::vector<std::vector<float>> &X);

std::string fmt_mat2d(const std::vector<std::vector<float>> &X);
