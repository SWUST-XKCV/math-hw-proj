#pragma once

#include <algorithm>
#include <limits>
#include <vector>

class RegressionTree {
public:
  struct Node {
    bool is_leaf;
    float value;
    int feat_idx;
    float threshold;
    Node *left;
    Node *right;

    Node()
        : is_leaf(false), value(0), feat_idx(-1), threshold(0), left(nullptr),
          right(nullptr) {}
  };

private:
  Node *m_root;
  int m_max_depth;
  int m_min_samples_split;

  Node *build_tree(const std::vector<std::vector<float>> &X,
                   const std::vector<float> &y, int depth) {
    if (X.size() <= m_min_samples_split || depth >= m_max_depth) {
      // 创建叶节点
      Node *leaf = new Node();
      leaf->is_leaf = true;
      leaf->value = calc_mean(y); // 均值
      return leaf;
    }

    // 寻找最佳分割
    int best_feat;
    float best_threshold;
    float best_mse = std::numeric_limits<float>::max();
    std::vector<std::vector<float>> best_left_x, best_right_x;
    std::vector<float> best_left_y, best_right_y;

    for (int feat = 0; feat < X[0].size(); ++feat) {
      // 按特征排序
      std::vector<std::pair<float, float>> feat_values;
      for (size_t i = 0; i < X.size(); ++i) {
        feat_values.push_back({X[i][feat], y[i]});
      }

      std::sort(feat_values.begin(), feat_values.end());

      // 尝试每个分割阈值
      for (size_t i = 1; i < feat_values.size(); ++i) {
        float threshold = (feat_values[i - 1].first + feat_values[i].first) / 2;
        std::vector<std::vector<float>> left_x, right_x;
        std::vector<float> left_y, right_y;

        for (size_t j = 0; j < X.size(); ++j) {
          if (X[j][feat] <= threshold) {
            left_x.push_back(X[j]);
            left_y.push_back(y[j]);
          } else {
            right_x.push_back(X[j]);
            right_y.push_back(y[j]);
          }
        }

        // 计算MSE
        float mse = calc_mse(left_y, right_y);
        if (mse < best_mse) {
          best_mse = mse;
          best_feat = feat;
          best_threshold = threshold;
          best_left_x = left_x;
          best_left_y = left_y;
          best_right_x = right_x;
          best_right_y = right_y;
        }
      }
    }

    // 创建内部节点
    Node *node = new Node();
    node->is_leaf = false;
    node->feat_idx = best_feat;
    node->threshold = best_threshold;
    node->left = build_tree(best_left_x, best_left_y, depth + 1);
    node->right = build_tree(best_right_x, best_right_y, depth + 1);

    return node;
  }

  float predict_node(Node *node, const std::vector<float> &x) const {
    if (node->is_leaf) {
      return node->value;
    }

    if (x[node->feat_idx] <= node->threshold) {
      return predict_node(node->left, x);
    } else {
      return predict_node(node->right, x);
    }
  }

  float calc_mean(const std::vector<float> &y) {
    float sum = 0;
    for (float value : y) {
      sum += value;
    }
    return sum / y.size();
  }

  float calc_mse(const std::vector<float> &leftY,
                 const std::vector<float> &rightY) {
    float mse = 0;

    // 左子树的MSE
    float meanLeft = calc_mean(leftY);
    for (float value : leftY) {
      mse += (value - meanLeft) * (value - meanLeft);
    }

    // 右子树的MSE
    float meanRight = calc_mean(rightY);
    for (float value : rightY) {
      mse += (value - meanRight) * (value - meanRight);
    }

    return mse;
  }

public:
  RegressionTree(size_t max_depth = 10, size_t min_samples_split = 2)
      : m_max_depth(max_depth), m_min_samples_split(min_samples_split) {};

  void train(const std::vector<std::vector<float>> &X,
             const std::vector<float> &y);
  float predict(const std::vector<float> &x) const;
};
