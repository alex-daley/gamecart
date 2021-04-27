#include <stdexcept>
#include "sqlite/sqlite3.h"
#include "database.h"

RowReader::RowReader(sqlite3* db, sqlite3_stmt* stmt) :
    database(db), stmt(stmt) 
{
}

RowReader::~RowReader() 
{
    if (stmt)
        sqlite3_finalize(stmt);
}

RowReader::operator bool() const 
{
    return sqlite3_errcode(database) == SQLITE_ROW;
}

bool RowReader::step() const 
{
    return sqlite3_step(stmt) == SQLITE_ROW;
}

int RowReader::getInteger(int column) const 
{
    return sqlite3_column_int(stmt, column);
}

double RowReader::getDouble(int column) const 
{
    return sqlite3_column_double(stmt, column);
}

std::string RowReader::getText(int column) const 
{
    const unsigned char* text = sqlite3_column_text(stmt, column);
    if (!text)
    {
        std::string info = sqlite3_errmsg(database);
        throw std::runtime_error("Failed get text: " + info);
    }

    return reinterpret_cast<const char*>(text);
}

Statement::Statement(sqlite3* db, std::string sql) 
{
    database = db;
    if (sqlite3_prepare_v2(database, sql.c_str(), sql.length(), &stmt, 0) != SQLITE_OK) 
    {
        std::string info = sqlite3_errmsg(database);
        throw std::runtime_error("Failed to prepare statement: " + info);
    }
}

Statement::~Statement() 
{
    if (stmt && !deferFree)
    {
        sqlite3_finalize(stmt);
    }
}

Statement& Statement::bindInteger(int integer) 
{
    sqlite3_bind_int(stmt, ++bindIndex, integer);
    return *this;
}

Statement& Statement::bindDouble(double value) 
{
    sqlite3_bind_double(stmt, ++bindIndex, value);
    return *this;
}

Statement& Statement::bindText(std::string text) 
{
    sqlite3_bind_text(stmt, ++bindIndex, text.c_str(), text.size() + 1, SQLITE_TRANSIENT);
    return *this;
}

RowReader Statement::execute() 
{
    int err = sqlite3_step(stmt);
    if (!(err == SQLITE_DONE || err == SQLITE_ROW))
    {
        std::string info = sqlite3_errmsg(database);
        throw std::runtime_error("Failed to step prepared statement: " + info);
    }

    deferFree = true;
    return RowReader(database, stmt);
}

Database::Database(const std::string& file) 
{
    if (sqlite3_open(file.c_str(), &database) != SQLITE_OK) 
    {
        std::string info = sqlite3_errmsg(database);
        throw std::runtime_error("Failed to open database: " + info);
    }
}

Database::~Database() 
{
    if (database) 
    {
        sqlite3_close(database);
    }
}

Statement Database::prepare(const std::string& sql) const 
{
    return Statement(database, sql);
}
