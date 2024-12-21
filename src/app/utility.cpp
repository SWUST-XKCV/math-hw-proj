#include <algorithm>
#include <app/regression_tree.hpp>
#include <app/student.hpp>
#include <app/utility.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>

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

std::string fmt_mat(const std::vector<std::vector<float>> &X) {
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

std::vector<Student> load_data_from_csv(const std::string &path) {
  std::vector<Student> stus;
  std::ifstream file(path);
  std::string line;

  // Skip the header line
  // stu, tm, x1, x2, ..., x9
  std::getline(file, line);

  while (std::getline(file, line)) {
    std::map<std::string, std::vector<float>> scores;
    std::string stu_name;
    std::string stu_tm;
    std::string value;
    std::vector<float> tm_scores;
    std::istringstream iss(line);
    // stu
    std::getline(iss, stu_name, ',');
    // tm
    std::getline(iss, stu_tm, ',');
    // x1, x2, ..., x9
    for (size_t i = 0; i < 9; i++) {
      std::getline(iss, value, ',');
      tm_scores.push_back(std::stof(value));
    }
    scores.insert(std::make_pair(stu_tm, tm_scores));
    for (size_t i = 0; i < 5; i++) {
      tm_scores.clear();
      std::getline(file, line);
      std::istringstream ss(line);
      // stu
      std::getline(ss, stu_name, ',');
      // tm
      std::getline(ss, stu_tm, ',');
      // x1, x2, ..., x9
      for (size_t i = 0; i < 9; i++) {
        std::getline(ss, value, ',');
        tm_scores.push_back(std::stof(value));
      }
      scores.insert(std::make_pair(stu_tm, tm_scores));
    }

    stus.push_back(Student(stu_name, scores));
  }

  return stus;
}

std::vector<size_t>
gen_missing_sample_idxs(const std::vector<std::vector<float>> &X) {
  std::vector<size_t> idxs;
  size_t n_sample = X.size();
  size_t n_feat = X[0].size();

  for (auto i = 0; i < n_sample; i++) {
    for (auto j = 0; j < n_feat; j++) {
      if (std::abs(X[i][j] - (-1.0)) < 0.00001) {
        idxs.push_back(i);
        break;
      }
    }
  }

  return idxs;
}

std::vector<size_t>
gen_missing_feat_idxs(const std::vector<std::vector<float>> &X) {
  std::vector<size_t> idxs;
  size_t n_sample = X.size();
  size_t n_feat = X[0].size();

  for (auto i = 0; i < n_feat; i++) {
    for (auto j = 0; j < n_sample; j++) {
      if (std::abs(X[j][i] - (-1.0)) < 0.00001) {
        idxs.push_back(i);
        break;
      }
    }
  }

  return idxs;
}

void impute_missing_values(std::vector<Student> &stus) {
  std::vector<std::vector<float>> X_all = ::stus_to_mat(stus);
  std::vector<std::vector<float>> X_all_meaned =
      ::impute_missing_by_mean(X_all);
  std::vector<RegressionTree> trees_all;
  size_t n_all_samples = X_all.size();
  size_t n_feat = X_all[0].size();

  ::printf("[INFO] trees_all is training...\n");

  for (auto i = 0; i < n_feat; i++) {
    auto X = X_all;
    std::vector<float> y;
    for (auto &x : X) {
      y.push_back(x[i]);
      x.erase(x.begin() + i);
    }
    auto tree = RegressionTree(3, 2);
    tree.train(X, y);
    trees_all.push_back(tree);
    ::printf("[INFO] trees_all[%d] has trained.\n", i);
  }

  ::printf("[INFO] trees_all has trained.\n");

  for (auto &stu : stus) {
    std::vector<std::vector<float>> X = ::stu_to_mat(stu);
    std::vector<std::vector<float>> X_output = X;
    std::vector<std::vector<float>> X_meaned = ::impute_missing_by_mean(X);
    std::vector<size_t> missing_sample_idxs = ::gen_missing_sample_idxs(X);
    std::vector<size_t> missing_feat_idxs = ::gen_missing_feat_idxs(X);
    size_t n_sample = X.size();
    auto tree = RegressionTree(2, 2);

    ::printf("[DEBUG] stu.name = %s\n", stu.m_name.c_str());
    ::printf("[DEBUG] missing_feat_idxs.size = %lld\n",
             missing_feat_idxs.size());
    ::printf("[DEBUG] missing_sample_idxs.size = %lld\n",
             missing_sample_idxs.size());

    for (auto i : missing_feat_idxs) {
      auto X_erased = X_meaned;
      auto y = std::vector<float>();
      auto p = std::vector<float>();
      auto tree = RegressionTree(2, 2);

      for (auto &x : X_erased) {
        y.push_back(x[i]);
        x.erase(x.begin() + i);
      }

      tree.train(X_erased, y);

      for (auto j = 0; j < n_sample; j++) {
        if (std::abs(X_output[j][i] - (-1.0)) < 0.00001) {
          X_output[j][i] = 0.8 * tree.predict(X_erased[j]) +
                           0.2 * trees_all[i].predict(X_erased[j]);
        }
      }

      stu.from_mat(X_output);
    }
  }
}

void sort(std::vector<Student> &stus) {
  auto cmp = [](const Student &stu1, const Student &stu2) {
    return stu1.calc_weighted_score() > stu2.calc_weighted_score();
  };

  std::sort(stus.begin(), stus.end(), cmp);
}

std::vector<std::vector<float>> stu_to_mat(const Student &stu) {
  std::vector<std::vector<float>> X;

  for (auto &i : stu.m_scores) {
    X.push_back(i.second);
  }

  return X;
}

std::vector<std::vector<float>> stus_to_mat(const std::vector<Student> &stus) {
  std::vector<std::vector<float>> mat;

  for (auto &stu : stus) {
    auto X = stu_to_mat(stu);
    for (auto &x : X) {
      mat.push_back(x);
    }
  }

  return mat;
}
