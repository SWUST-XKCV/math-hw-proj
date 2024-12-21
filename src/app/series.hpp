#pragma once

#include <string>
#include <vector>

template <typename DType> struct Series {
private:
  static const std::string dtype_name;
  std::string m_name;
  std::vector<DType> m_data;

public:
  Series(const std::string &name, const std::vector<float> &data) {
    m_name = name;
    m_data = data;
  }

  Series(const std::string &name, const std::vector<float> &&data) {
    m_name = name;
    m_data = data;
  }

  Series<DType> &operator=(const Series<DType> &other) {
    m_name = other.m_name;
    m_data = other.m_data;

    return *this;
  }

  Series<DType> &operator=(Series<DType> &&other) {
    m_name = other.m_name;
    m_data = other.m_data;

    other.m_name.clear();
    other.m_data.clear();

    return *this;
  }

  size_t len() { return m_data.size(); }

  size_t size() { return len(); }

  DType &operator[](size_t idx) { return m_data[idx]; }

  std::string debug() {
    std::string str = m_name;
    str += ", " + Series<DType>::dtype_name + ": [";
    size_t n = len();

    str += std::to_string(m_data[0]);

    for (auto i = 1; i < n; i++) {
      if (i % 10 == 0) {
        str += '\n';
      }

      str += std::to_string(m_data[i]);
    }

    str += "]";

    return str;
  }
};

template <typename DType>
const std::string Series<DType>::dtype_name = "Unknown Type";
