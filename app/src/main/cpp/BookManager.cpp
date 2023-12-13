// BookManager.cpp
#include "BookManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <android/log.h>
#include "logging.h"  // Certifique-se de incluir o cabeçalho para logging

#define TAG "app"

BookManager::BookManager() {
    // Caminho para o arquivo CSV interno no dispositivo
    csvFilePath = "/sdcard/Download/favorite_books.csv";
}

void BookManager::saveBookToCSV(const Book& book) {
    // Abre o arquivo CSV para leitura
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Usando um set para armazenar IDs únicos
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extrai a ID da linha
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {
                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());

                // Exibe a ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "ID existente no CSV: %s", currentId.c_str());
                // Adiciona a ID ao conjunto
                existingIds.insert(currentId);
            }
        }
        // Fecha o arquivo de leitura
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Erro ao abrir o arquivo CSV para leitura");
    }
    // Verifica se a ID já existe no conjunto

    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXXX saveBookToCSV ID=  %s", book.id.c_str());

    if (existingIds.find(book.id) != existingIds.end()) {
        // A ID já existe, não é necessário adicionar novamente
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Livro com ID %s já existe no CSV", book.id.c_str());
        return;
    }
    // Abre o arquivo CSV para adicionar novos dados
    std::ofstream csvWriteFile(csvFilePath, std::ios::app);
    if (csvWriteFile.is_open()) {
        // Verifica se a ID já existe no conjunto
        if (existingIds.find(book.id) != existingIds.end()) {
            // A ID já existe, não é necessário adicionar novamente
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Livro com ID %s já existe no CSV", book.id.c_str());
        } else{
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Livro novo %s no CSV", book.id.c_str());
            csvWriteFile << book.id + "|" << "," << book.title + "|" << "," << book.description  + "|" << "," << book.smallThumbnail << + "|" "\n";
        }
        // Fecha o arquivo
        csvWriteFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Erro ao abrir o arquivo CSV para escrita");
    }
}

std::vector<Book> BookManager::getFavoriteBooksFromCSV() {
    std::vector<Book> favoriteBooks;

    // Abre o arquivo CSV para leitura
    std::ifstream csvReadFile(csvFilePath);

    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Encontrar as posições dos pipes (|)
            size_t idPos = 0;
            size_t titlePos = line.find('|', idPos);
            size_t descriptionPos = line.find('|', titlePos + 1);
            size_t imagePos = line.find('|', descriptionPos + 1);

            // Verificar se as posições são válidas
            if (titlePos != std::string::npos && descriptionPos != std::string::npos && imagePos != std::string::npos) {
                // Extrair os detalhes do livro da linha
                std::string id = line.substr(idPos, titlePos - idPos);
                std::string title = line.substr(titlePos + 1, descriptionPos - titlePos - 1);
                std::string description = line.substr(descriptionPos + 1, imagePos - descriptionPos - 1);
                std::string image = line.substr(imagePos + 1);

                // Remover vírgulas indesejadas
                title.erase(std::remove(title.begin(), title.end(), ','), title.end());
                description.erase(std::remove(description.begin(), description.end(), ','), description.end());
                image.erase(std::remove(image.begin(), image.end(), ','), image.end());

                // Remover caractere "|" no final da imagem
                if (!image.empty() && image[image.length() - 1] == '|') {
                    image.pop_back();
                }

                // Crie uma instância de Book com um construtor adequado e adicione à lista de favoritos
                Book bk(id.c_str(), title.c_str(), description.c_str(), image.c_str());
                favoriteBooks.push_back(bk);

                // Log para verificar
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", id.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", title.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", description.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", image.c_str());
            }
        }

        // Fechar o arquivo de leitura
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Erro ao abrir o arquivo CSV para leitura");
    }

    return favoriteBooks;
}

bool BookManager::checkIDinCSV(std::string id) {
    // Abre o arquivo CSV para leitura
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Usando um set para armazenar IDs únicos
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extrai a ID da linha
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {

                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());

                // Exibe a ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "ID existente no CSV: %s", currentId.c_str());
                // Adiciona a ID ao conjunto
                existingIds.insert(currentId);
            }
        }
        // Fecha o arquivo de leitura
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Erro ao abrir o arquivo CSV para leitura");
        return false; // Se houver erro ao abrir o arquivo, consideramos como ID não existente
    }
    // Verifica se a ID já existe no conjunto
    return existingIds.find(id) != existingIds.end();
}

bool BookManager::checkElementsCSV() {
    // Abre o arquivo CSV para leitura
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Usando um set para armazenar IDs únicos
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extrai a ID da linha
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {

                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());
                // Exibe a ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "checkElementsCSV ID existente no CSV: %s", currentId.c_str());
                if (!currentId.empty()) {
                    // Se pelo menos um ID não estiver vazio, retornar true
                    csvReadFile.close();
                    return true;
                }
            }
        }
        // Fecha o arquivo de leitura
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Erro ao abrir o arquivo CSV para leitura");
        return false; // Se houver erro ao abrir o arquivo, consideramos como ID não existente
    }

    // Se nenhum ID não vazio foi encontrado, retornar false
    return false;
}
