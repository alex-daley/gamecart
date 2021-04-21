#pragma once
#include <string>
#include <stdexcept>

struct sqlite3;
struct sqlite3_stmt;

class RowReader
{
public:
    RowReader(sqlite3* db, sqlite3_stmt* stmt);
    ~RowReader();

    // Implicit conversion to bool. Returns true if a row is present
    operator bool() const;

    bool step() const;
    int get_int(int column) const;
    std::string get_text(int column) const;

private:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
};

class Statement
{
public:
    Statement(sqlite3* db, std::string sql);
    ~Statement();

    Statement& bind_integer(int integer);
    Statement& bind_text(std::string text);
    RowReader execute();

private:
    bool m_defer_free = false;
    int m_bind_index = 0;
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
};

class Database
{
public:
    Database(std::string file);
    ~Database();

    Statement prepare(std::string sql) const;

private:
    sqlite3* m_db;
};
