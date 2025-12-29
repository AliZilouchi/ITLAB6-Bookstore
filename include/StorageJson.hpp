#pragma once
#include <string>
#include <vector>
#include "Book.hpp"

namespace storage {

// Loads from filePath. If file doesn't exist -> empty list, ok=true.
// If parse fails -> empty list, ok=false.
std::vector<Book> loadBooksJson(const std::string& filePath, bool* ok = nullptr);

bool saveBooksJson(const std::string& filePath, const std::vector<Book>& books);

} // namespace storage
