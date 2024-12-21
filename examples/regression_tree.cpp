#include <app/regression_tree.hpp>
#include <app/utility.hpp>

#include <cstdio>
#include <cstdlib>
#include <vector>

std::vector<float> predict(const RegressionTree &tree,
                           const std::vector<std::vector<float>> &X) {
  std::vector<float> y_hat;

  for (auto &sample : X) {
    y_hat.push_back(tree.predict(sample));
  }

  return y_hat;
}

int main() {
  ::printf("Regression Tree Example.\n");

  std::vector<std::vector<float>> data0 = {
      {109, 121, 131, 71, 81, 77, 77, 81, 91},
      {110, 134, 139, 75, 82, 72, 76, 72, 92},
      {115, 129, 139, -1, -1, 77, 88, 77, 89},
      {106, -1, -1, -1, 88, 72, -1, 75, 88},
      {109, 131, 137, -1, 79, 78, -1, 78, 89},
      {111, 135, 134, 69, 70, 79, -1, 79, 90},
  };

  size_t n_sample = data0.size();
  size_t n_feat = data0[0].size();

  auto output_data = data0;
  auto new_data = ::impute_missing_by_mean(data0);

  ::printf("%s\n", ::fmt_mat2d(new_data).c_str());

  std::vector<size_t> missing_feat_idxs = {1, 2, 3, 4, 6};
  std::vector<size_t> missing_sample_idxs = {2, 3, 4, 5};

  for (auto i : missing_feat_idxs) {
    auto X = new_data;
    auto p = std::vector<float>();
    auto y = std::vector<float>();
    auto tree = RegressionTree(2, 2);
    auto missing_samples = std::vector<std::vector<float>>();

    for (auto j = 0; j < n_sample; j++) {
      y.push_back(X[j][i]);
      X[j].erase(X[j].begin() + i);
    }

    tree.train(X, y);

    for (auto j = 0; j < n_sample; j++) {
      if (std::abs(data0[j][i] - (-1.0)) < 0.00001) {
        missing_samples.push_back(X[j]);
        output_data[j][i] = tree.predict(X[j]);
      }
    }

    p = ::predict(tree, missing_samples);
    ::printf("missing_feat_idx %lld: [\n", i);

    for (auto j : p) {
      ::printf("\t%f\n", j);
    }

    ::printf("]\n");
  }

  ::printf("%s\n", ::fmt_mat2d(output_data).c_str());

  return 0;
}
