#include "CliUtil.hpp"
#include "Library.hpp"
#include <iostream>

static void printBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books.\n";
        return;
    }
    std::cout << "ID | Year | Title - Author\n";
    std::cout << "---------------------------\n";
    for (const auto& b : books) {
        std::cout << b.id << " | " << b.year << " | " << b.title << " - " << b.author << "\n";
    }
}

int main() {
    const std::string kFileName = "books.json";

    Library lib;
    lib.load(kFileName);

    while (true) {
        std::cout << "\n=== Arash Library ===\n";
        std::cout << "1) List books\n";
        std::cout << "2) Add book\n";
        std::cout << "3) Delete book\n";
        std::cout << "4) Search books\n";
        std::cout << "5) Save\n";
        std::cout << "6) Exit\n";

        int choice = cli::readInt("Select: ");

        if (choice == 1) {
            printBooks(lib.all());
        } else if (choice == 2) {
            std::string title = cli::readLine("Title: ");
            std::string author = cli::readLine("Author: ");
            int year = cli::readInt("Year: ");

            int id = lib.addBook(title, author, year);
            std::cout << "Added. ID = " << id << "\n";
            if (!lib.save(kFileName)) std::cout << "Warning: could not save to books.json\n";

        } else if (choice == 3) {
            std::cout << "Delete by:\n";
            std::cout << "1) ID\n";
            std::cout << "2) Exact Title\n";
            int ch = cli::readInt("Choice: ");

            if (ch == 1) {
                int id = cli::readInt("ID: ");
                if (lib.deleteById(id)) std::cout << "Deleted.\n";
                else std::cout << "No book with that ID.\n";
            } else if (ch == 2) {
                std::string title = cli::readLine("Exact Title: ");
                int removed = lib.deleteByExactTitle(title);
                if (removed > 0) std::cout << "Deleted " << removed << " book(s).\n";
                else std::cout << "No book with that exact title.\n";
            } else {
                std::cout << "Invalid choice.\n";
            }

            if (!lib.save(kFileName)) std::cout << "Warning: could not save to books.json\n";

        } else if (choice == 4) {
            std::string q = cli::readLine("Search text (title/author): ");
            auto matches = lib.search(q);
            if (matches.empty()) std::cout << "No matches.\n";
            else printBooks(matches);

        } else if (choice == 5) {
            if (lib.save(kFileName)) std::cout << "Saved to books.json\n";
            else std::cout << "Save failed.\n";

        } else if (choice == 6) {
            lib.save(kFileName); // auto-save on exit
            std::cout << "Bye.\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
