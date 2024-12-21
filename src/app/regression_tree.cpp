#include <app/regression_tree.hpp>

void RegressionTree::train(const std::vector<std::vector<float>> &X,
                           const std::vector<float> &y) {
  m_root = build_tree(X, y, 0);
}

float RegressionTree::predict(const std::vector<float> &x) const {
  return predict_node(m_root, x);
}
