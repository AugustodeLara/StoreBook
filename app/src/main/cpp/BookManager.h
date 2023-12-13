// BookManager.h
#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include <string>
#include <vector>
#include <set>
#include "Book.h"  // Certifique-se de incluir o cabeçalho do Book

class BookManager {
public:
    BookManager();
    void saveBookToCSV(const Book& book);
    bool checkIDinCSV(std::string id);
    bool checkElementsCSV();
    std::vector<Book> getFavoriteBooksFromCSV();
    std::string csvFilePath;

private:
};

#endif  // BOOK_MANAGER_H
