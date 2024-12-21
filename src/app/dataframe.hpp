#pragma once

#include <app/db.hpp>
#include <app/series.hpp>

#include <string_view>
#include <variant>

class DataFrame {
private:
  using ColType = std::variant<Series<float>, Series<std::string>>;

  std::vector<ColType> m_cols;

public:
  static DataFrame from_csv(const std::string_view &file_path);
  void save_to_db(const Database *db);

  DataFrame();
};
