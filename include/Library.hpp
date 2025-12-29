#pragma once
#include <string>
#include <vector>
#include "Book.hpp"

class Library {
public:
    void load(const std::string& filePath);   // loads into memory
    bool save(const std::string& filePath) const;

    int addBook(std::string title, std::string author, int year); // returns new id
    bool deleteById(int id);
    int deleteByExactTitle(const std::string& titleExact);        // returns count removed

    std::vector<Book> search(const std::string& query) const;     // title/author partial match
    const std::vector<Book>& all() const;

private:
    std::vector<Book> books_;
    int nextId_ = 1;

    void recomputeNextId();
};
