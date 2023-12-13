#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <atomic>

    class Book {
    public:

        Book(const char* id, const char* title, const char* description, const char* smallThumbnail, bool favorite,const char* buyLink);
        Book(const char* id, const char* title, const char* description, const char* smallThumbnail);


        std::string id;
        std::string title;
        std::string subtitle;
        std::vector<std::string> authors;
        std::string description;
        std::string smallThumbnail;
        bool favorite;
        std::string buyLink;


        // Adicione outros campos conforme necessário

        // Construtor padrão
        Book();

        // Construtor com parâmetros
        Book(const std::string& id, const std::string& title, const std::string& subtitle,
             const std::vector<std::string>& authors, const std::string& description,
             const std::string& smallThumbnail, const std::string& buyLink);


        // Construtor com parâmetros
        Book(const std::string& id, const std::string& title, const std::string& description,
             const std::string& smallThumbnail);


        // Destrutor
        ~Book();
    };



#endif // BOOK_H
