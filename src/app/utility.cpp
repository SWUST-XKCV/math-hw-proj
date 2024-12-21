#include <app/utility.hpp>
#include <cstdlib>

std::vector<std::vector<float>>
impute_missing_by_mean(const std::vector<std::vector<float>> &X) {
  std::vector<std::vector<float>> X_new = X;
  std::vector<float> avarage_feat;
  size_t n_sample = X.size();
  size_t n_feat = X[0].size();

  for (int i = 0; i < n_feat; i++) {
    float sum = 0.0;
    int n_valid = 0;
    for (int j = 0; j < n_sample; j++) {
      if (!(std::abs(X[j][i] - (-1.0)) < 0.00001)) {
        n_valid += 1;
        sum += X[j][i];
      }
    }
    avarage_feat.push_back(sum / n_valid);
  }

  for (int i = 0; i < n_feat; i++) {
    for (int j = 0; j < n_sample; j++) {
      if (std::abs(X_new[j][i] - (-1.0)) < 0.00001) {
        X_new[j][i] = avarage_feat[i];
      }
    }
  }

  return X_new;
}

std::string fmt_mat2d(const std::vector<std::vector<float>> &X) {
  std::string str;
  size_t n = X.size();
  size_t m = X[0].size();

  for (auto i = 0; i < n; i++) {
    str += std::to_string(X[i][0]);
    for (auto j = 1; j < m; j++) {
      str += ", " + std::to_string(X[i][j]);
    }
    str += ";\n";
  }

  return str;
}
