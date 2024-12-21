#include <app/student.hpp>
#include <app/utility.hpp>

#include <cstdio>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<Student> stus = ::load_data_from_csv("build/output.csv");
  size_t n_stus = stus.size();

  ::printf("stus.size = %lld\n", n_stus);

  ::impute_missing_values(stus);

  ::sort(stus);

  /*for (size_t i = 0; i < n_stus; i++) {*/
  /*  ::printf("o%lld: %s\n", i + 1, stus[i].fmt().c_str());*/
  /*}*/

  return 0;
}
