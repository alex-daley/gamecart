#pragma once

#include <filesystem>
#include <atomic>
#include <string>

#include <database.h>
#include <schema.h>

class MockDatabase
{
public:
    MockDatabase()
    {
        filename = "test" + std::to_string(++count) + ".db";
        database = new Database(filename);
        Schema::createTables(*database, false);
    }

    // Teardown.
    ~MockDatabase()
    {
        delete database;
        std::filesystem::remove(filename);
    }

    Database* getDatabase() const
    {
        return database;
    }

private:
    inline static std::atomic<int> count = 0;

    std::string filename;
    Database* database;
};
