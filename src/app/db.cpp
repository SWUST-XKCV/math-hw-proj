#include <app/db.hpp>

Database *Database::db = nullptr;
std::mutex Database::mtx;

void Database::init(const std::string_view &db_file_path) {
  m_db_file_path = std::string(db_file_path);
}
