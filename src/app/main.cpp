#include <app/student.hpp>

#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

void imputs_missing_value(std::vector<Student> stus) {}

void sort(std::vector<Student> stus) {}

std::vector<Student> read_csv(const std::string &path) {
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

int main(int argc, char *argv[]) {
  std::vector<Student> stus;
  stus = read_csv("build/output.csv");
  size_t n_stus = stus.size();

  ::printf("stus.size = %lld\n", n_stus);

  ::imputs_missing_value(stus);

  ::sort(stus);

  /*for (size_t i = 0; i < n_stus; i++) {*/
  /*  ::printf("o%lld: %s\n", i + 1, stus[i].fmt().c_str());*/
  /*}*/

  return 0;
}
