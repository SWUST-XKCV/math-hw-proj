#pragma once

#include <cstdio>
#include <map>
#include <string>
#include <vector>

struct Student {
  std::string m_name;
  std::map<std::string, std::vector<float>> m_scores;

  Student() {};

  Student(const std::string &name,
          const std::map<std::string, std::vector<float>> scores) {
    m_name = name;
    m_scores = scores;
  }

  float calc_weighted_score() const {
    float ans = 0.0;
    std::vector<float> w_tm = {0.1, 0.15, 0.2, 0.15, 0.2, 0.2};
    std::vector<float> w_subject = {0.14, 0.13, 0.13, 0.1, 0.1,
                                    0.1,  0.1,  0.1,  0.1};
    auto mat = as_mat();

    for (int i = 0; i < 6; i++) {
      float sum = 0.0;
      for (int j = 0; j < 9; j++) {
        sum += w_subject[j] * mat[i][j];
      }
      ans += w_tm[i] * sum;
    }

    return ans;
  }

  std::string fmt() const {
    std::string str = m_name + ": [\n";

    for (auto &i : m_scores) {
      str += i.first + ": ";
      size_t n = i.second.size();
      str += std::to_string(i.second[0]);
      for (auto j = 1; j < n; j++) {
        str += std::string(" ,") + std::to_string(i.second[j]);
      }
      str += '\n';
    }

    str += "]";

    return str;
  }

  Student &from_mat(const std::vector<std::vector<float>> &mat) {
    size_t n = 0;
    m_scores.clear();

    for (auto &row : mat) {
      m_scores.insert(
          std::make_pair(std::string("t") + std::to_string(++n), row));
    }

    return *this;
  }

  std::vector<std::vector<float>> as_mat() const {
    std::vector<std::vector<float>> X;

    for (auto &i : m_scores) {
      X.push_back(i.second);
    }

    return X;
  };
};
