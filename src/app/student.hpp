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

  std::string fmt() {
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

  std::vector<std::vector<float>> as_mat() {
    std::vector<std::vector<float>> X;

    for (auto &i : m_scores) {
      X.push_back(i.second);
    }

    return X;
  };
};
