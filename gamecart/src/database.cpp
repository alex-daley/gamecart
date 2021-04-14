#include <stdexcept>
#include "sqlite/sqlite3.h"
#include "database.hpp"

RowReader::RowReader(sqlite3_stmt* stmt)
{
    m_stmt = stmt;
}

RowReader::~RowReader()
{
    if (m_stmt)
    {
        sqlite3_finalize(m_stmt);
    }
}

bool RowReader::step() const
{
    return sqlite3_step(m_stmt) == SQLITE_ROW;
}

int RowReader::get_int(int column) const
{
    return sqlite3_column_int(m_stmt, column);
}

std::string RowReader::get_text(int column) const 
{
    const unsigned char* text = sqlite3_column_text(m_stmt, column);
    return reinterpret_cast<const char*>(text);
}

Statement::Statement(sqlite3* db, std::string sql)
{
    m_db = db;
    
    if (sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &m_stmt, 0) != SQLITE_OK)
    {
        std::string info = sqlite3_errmsg(m_db);
        throw std::runtime_error("Failed to prepare statement: " + info);
    }
}

Statement::~Statement()
{
    if (m_stmt && !m_defer_free)
    {
        sqlite3_finalize(m_stmt);
    }
}

Statement& Statement::bind_integer(int integer)
{
    sqlite3_bind_int(m_stmt, ++m_bind_index, integer);
    return *this;
}

Statement& Statement::bind_text(std::string text)
{
    sqlite3_bind_text(m_stmt, ++m_bind_index,text.c_str(), text.size() + 1, SQLITE_TRANSIENT);
    return *this;
}

RowReader Statement::execute()
{
    int err = sqlite3_step(m_stmt);
    if (!(err == SQLITE_DONE || err == SQLITE_ROW))
    {
        std::string info = sqlite3_errmsg(m_db);
        throw std::runtime_error("Failed to step prepared statement: " + info);
    }

    m_defer_free = true;
    return RowReader(m_stmt);
}

Database::Database(std::string file)
{
    if (sqlite3_open("gamecart.db", &m_db) != SQLITE_OK)
    {
        std::string info = sqlite3_errmsg(m_db);
        throw std::runtime_error("Failed to open database: " + info);
    }
}

Database::~Database()
{
    if (m_db)
    {
        sqlite3_close(m_db);
    }
}

Statement Database::prepare(std::string sql) const
{
    return Statement(m_db, sql);
}
