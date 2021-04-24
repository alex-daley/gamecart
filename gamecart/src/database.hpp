#pragma once
#include <string>
#include <stdexcept>

struct sqlite3;
struct sqlite3_stmt;

class RowReader {
public:
    RowReader(sqlite3* db, sqlite3_stmt* stmt);
    ~RowReader();

    // Implicit conversion to bool. Returns true if a row is present
    operator bool() const;

    bool step() const;
    int get_int(int column) const;
    double get_double(int column) const;
    std::string get_text(int column) const;

private:
    sqlite3* db_;
    sqlite3_stmt* stmt_;
};

class Statement {
public:
    Statement(sqlite3* db, std::string sql);
    ~Statement();

    Statement& bind_integer(int integer);
    Statement& bind_double(double value);
    Statement& bind_text(std::string text);
    RowReader execute();

private:
    bool defer_free_ = false;
    int bind_index_ = 0;
    sqlite3* db_;
    sqlite3_stmt* stmt_;
};

class Database {
public:
    Database(std::string file);
    ~Database();

    Statement prepare(std::string sql) const;

private:
    sqlite3* db_;
};
