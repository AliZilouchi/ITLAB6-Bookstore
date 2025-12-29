#include "Library.hpp"
#include "StorageJson.hpp"
#include "StringUtil.hpp"
#include <algorithm>
#include <utility>

void Library::load(const std::string& filePath) {
    bool ok = true;
    books_ = storage::loadBooksJson(filePath, &ok);
    recomputeNextId();
    // If ok==false, books_ is empty; caller can decide how to message user.
}

bool Library::save(const std::string& filePath) const {
    return storage::saveBooksJson(filePath, books_);
}

void Library::recomputeNextId() {
    int mx = 0;
    for (const auto& b : books_) mx = std::max(mx, b.id);
    nextId_ = mx + 1;
}

int Library::addBook(std::string title, std::string author, int year) {
    Book b;
    b.id = nextId_++;
    b.title = std::move(title);
    b.author = std::move(author);
    b.year = year;
    books_.push_back(std::move(b));
    return books_.back().id;
}

bool Library::deleteById(int id) {
    auto it = std::remove_if(books_.begin(), books_.end(),
                             [&](const Book& b) { return b.id == id; });
    if (it == books_.end()) return false;
    books_.erase(it, books_.end());
    return true;
}

int Library::deleteByExactTitle(const std::string& titleExact) {
    int removed = 0;
    auto it = std::remove_if(books_.begin(), books_.end(), [&](const Book& b) {
        if (b.title == titleExact) { removed++; return true; }
        return false;
    });
    books_.erase(it, books_.end());
    return removed;
}

std::vector<Book> Library::search(const std::string& query) const {
    std::vector<Book> out;
    if (query.empty()) return out;

    for (const auto& b : books_) {
        if (strutil::icontains(b.title, query) || strutil::icontains(b.author, query)) {
            out.push_back(b);
        }
    }
    return out;
}

const std::vector<Book>& Library::all() const {
    return books_;
}
