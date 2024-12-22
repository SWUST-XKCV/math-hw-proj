#include "app/logger.hpp"
#include <app/student.hpp>
#include <app/ui/ui.hpp>
#include <app/utility.hpp>

int main(int argc, char *argv[]) {
  Logger::init("log.txt");

  ::ui_init();
  ::ui_loop();
  ::ui_close();

  return 0;
}
