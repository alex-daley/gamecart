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
    int getInteger(int column) const;
    double getDouble(int column) const;
    std::string getText(int column) const;

private:
    sqlite3* database;
    sqlite3_stmt* stmt;
};

class Statement 
{
public:
    Statement(sqlite3* db, std::string sql);
    ~Statement();

    Statement& bindInteger(int integer);
    Statement& bindDouble(double value);
    Statement& bindText(std::string text);
    RowReader execute();

private:
    bool deferFree = false;
    int bindIndex = 0;
    sqlite3* database;
    sqlite3_stmt* stmt;
};

class Database 
{
public:
    Database(const std::string& file);
    ~Database();

    Statement prepare(const std::string& sql) const;

private:
    sqlite3* database;
};
