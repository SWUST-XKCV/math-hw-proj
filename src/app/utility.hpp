#pragma once

#include "app/student.hpp"
#include <string>
#include <vector>

std::vector<std::vector<float>>
impute_missing_by_mean(const std::vector<std::vector<float>> &X);

std::string fmt_mat2d(const std::vector<std::vector<float>> &X);

void impute_missing_values(std::vector<Student> stus);

void sort(std::vector<Student> stus);

std::vector<Student> load_data_from_csv(const std::string &path);

std::vector<std::vector<float>> stu_to_mat2d(const Student &stu);

std::vector<std::vector<float>> stus_to_mat2d(const std::vector<Student> &stus);
