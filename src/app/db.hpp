#pragma once

#include <mutex>
#include <sqlite/sqlite3.h>
#include <string>
#include <string_view>

class Database {
private:
  static Database *db;
  static std::mutex mtx;

  std::string m_db_file_path;

  Database() {}

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

public:
  static Database *DB() {
    if (db == nullptr) {
      std::lock_guard<std::mutex> lock(mtx);
      if (db == nullptr) {
        db = new Database();
      }
    }

    return db;
  }

  void init(const std::string_view &db_file_path);
};
