// Book.cpp

#include "Book.h"

// Construtor com parâmetros
Book::Book(const char* id, const char* title, const char* description, const char* smallThumbnail, bool favorite, const char* buyLink)
        : id(id), title(title), description(description), smallThumbnail(smallThumbnail), favorite(favorite), buyLink(buyLink) {
    // Você pode inicializar outros campos aqui se necessário
}

// Construtor com parâmetros
Book::Book(const std::string& id, const std::string& title, const std::string& subtitle,
           const std::vector<std::string>& authors, const std::string& description,
           const std::string& smallThumbnail, const std::string& buyLink)
        : id(id), title(title), subtitle(subtitle), authors(authors),
          description(description), smallThumbnail(smallThumbnail), buyLink(buyLink) {
    // Você pode inicializar outros campos aqui se necessário
}


Book::Book(const char* id, const char* title, const char* description, const char* smallThumbnail)
        : id(id), title(title), description(description), smallThumbnail(smallThumbnail){
    // Você pode inicializar outros campos aqui se necessário
}



// Destrutor
Book::~Book() {
    // Adicione a lógica de destruição, se necessário
}

// Construtor padrão
Book::Book() : favorite(false) {
    // Você pode inicializar outros campos aqui se necessário
}
