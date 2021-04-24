#include <stdexcept>
#include "sqlite/sqlite3.h"
#include "database.hpp"

RowReader::RowReader(sqlite3* db, sqlite3_stmt* stmt) : 
    db_(db), stmt_(stmt) {
}

RowReader::~RowReader() {
    if (stmt_)
        sqlite3_finalize(stmt_);
}

RowReader::operator bool() const {
    return sqlite3_errcode(db_) == SQLITE_ROW;
}

bool RowReader::step() const {
    return sqlite3_step(stmt_) == SQLITE_ROW;
}

int RowReader::get_int(int column) const {
    return sqlite3_column_int(stmt_, column);
}

double RowReader::get_double(int column) const {
    return sqlite3_column_double(stmt_, column);
}

std::string RowReader::get_text(int column) const {
    const unsigned char* text = sqlite3_column_text(stmt_, column);
    return reinterpret_cast<const char*>(text);
}

Statement::Statement(sqlite3* db, std::string sql) {
    db_ = db;
    if (sqlite3_prepare_v2(db_, sql.c_str(), sql.length(), &stmt_, 0) != SQLITE_OK) {
        std::string info = sqlite3_errmsg(db_);
        throw std::runtime_error("Failed to prepare statement: " + info);
    }
}

Statement::~Statement() {
    if (stmt_ && !defer_free_)
        sqlite3_finalize(stmt_);
}

Statement& Statement::bind_integer(int integer) {
    sqlite3_bind_int(stmt_, ++bind_index_, integer);
    return *this;
}

Statement& Statement::bind_double(double value) {
    sqlite3_bind_double(stmt_, ++bind_index_, value);
    return *this;
}

Statement& Statement::bind_text(std::string text) {
    sqlite3_bind_text(stmt_, ++bind_index_, text.c_str(), text.size() + 1, SQLITE_TRANSIENT);
    return *this;
}

RowReader Statement::execute() {
    int err = sqlite3_step(stmt_);
    if (!(err == SQLITE_DONE || err == SQLITE_ROW))
    {
        std::string info = sqlite3_errmsg(db_);
        throw std::runtime_error("Failed to step prepared statement: " + info);
    }

    defer_free_ = true;
    return RowReader(db_, stmt_);
}

Database::Database(std::string file) {
    if (sqlite3_open("gamecart.db", &db_) != SQLITE_OK) {
        std::string info = sqlite3_errmsg(db_);
        throw std::runtime_error("Failed to open database: " + info);
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

Statement Database::prepare(std::string sql) const {
    return Statement(db_, sql);
}
