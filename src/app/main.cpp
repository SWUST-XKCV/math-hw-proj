#include <app/student.hpp>
#include <app/utility.hpp>

#include <cstdio>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<Student> stus = ::load_data_from_csv("build/output.csv");
  size_t n_stus = stus.size();

  ::printf("[INFO] stus.size = %lld\n", n_stus);

  ::impute_missing_values(stus);

  ::sort(stus);

  for (const auto &stu : stus) {
    ::printf("[INFO] %s weighted score = %f\n", stu.m_name.c_str(),
             stu.calc_weighted_score());
  }

  return 0;
}
