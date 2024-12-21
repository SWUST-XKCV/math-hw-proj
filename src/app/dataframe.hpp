#pragma once

#include <string_view>
class DataFrame {
  public:
      static DataFrame from_csv(const std::string_view& file_path);

      DataFrame();
};
