#include "http.h"

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <android/log.h>
#include <fstream>
#include <cstdio>  // Adicionado para manipulação de arquivos temporários
#include "Book.h"


#include "curl/curl.h"
#include "json/json.h"  // Certifique-se de que o caminho está correto

#define TAG "HTTP"

using namespace std::string_literals;

namespace {

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        size_t totalSize = size * nmemb;
        std::string* buffer = static_cast<std::string*>(userp);

        // Verifica se o buffer está vazio ou se o último caractere não é uma aspas dupla
        if (buffer->empty() || buffer->back() != '"') {
            buffer->append(static_cast<char*>(contents), totalSize);
        } else {
            // Se o buffer terminou com uma aspas dupla, significa que a descrição está sendo cortada
            // Nesse caso, adicionamos uma aspas simples no início para continuar a leitura da descrição
            buffer->pop_back();  // Remove a última aspas dupla
            buffer->push_back('\'');  // Adiciona uma aspas simples
            buffer->append(static_cast<char*>(contents), totalSize);
        }

        return totalSize;
    }

// Divide a string em partes menores e imprime no logcat
    void printToLogcat(const std::string& buffer) {
        //__android_log_print(ANDROID_LOG_VERBOSE, TAG, "printToLogcat tamanho %d",buffer.length());

        const size_t chunkSize = 1000;  // Tamanho máximo permitido pelo logcat
        for (size_t i = 0; i < buffer.length(); i += chunkSize) {
            std::string chunk = buffer.substr(i, chunkSize);
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Received data: %s", chunk.c_str());
        }
    }


    std::vector<Book> parseBooksFromJson(const std::string& json) {
        std::vector<Book> books;

        Json::CharReaderBuilder builder;
        std::istringstream jsonStream(json);
        Json::Value root;

        // Parse JSON
        if (Json::parseFromStream(builder, jsonStream, &root, nullptr)) {
            // Verifica se o objeto JSON contém a chave "items"
            if (root.isMember("items")) {
                // Obtém o array de livros
                const auto& items = root["items"];

                // Itera sobre os livros no array
                for (const auto& item : items) {
                    Book book;
                    book.id = item["id"].asString();

                    // Extract volumeInfo
                    const auto& volumeInfo = item["volumeInfo"];
                    book.title = volumeInfo["title"].asString();
                    book.subtitle = volumeInfo.get("subtitle", "").asString(); // Pode não existir um subtítulo

                    // Extract authors
                    const auto& authors = volumeInfo["authors"];
                    for (const auto& author : authors) {
                        book.authors.push_back(author.asString());
                    }

                    // Extract description
                    book.description = volumeInfo.get("description", "").asString();

                    // Extract smallThumbnail
                    book.smallThumbnail = volumeInfo["imageLinks"].get("smallThumbnail", "").asString();

                    // Extract buyLink from saleInfo or set "Indisponivel" if not present
                    if (item.isMember("saleInfo") && item["saleInfo"].isMember("buyLink")) {
                        book.buyLink = item["saleInfo"]["buyLink"].asString();
                        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX BuyLink: %s", book.buyLink.c_str());
                    } else {
                        book.buyLink = "Indisponivel";
                        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX BuyLink indisponivel");
                    }

                    // Adiciona o livro ao vetor
                    books.push_back(book);
                }
            }
        } else {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Error parsing JSON");
        }

        return books;
    }

}  // namespace

namespace curlssl {
    namespace http {

        Client::Client(const std::string &cacert_path)
                : cacert_path(cacert_path.empty() ? "/home/aglara/AndroidStudioProjects/bookStore/app/src/main/assets/cacert.pem" : cacert_path) {
            curl_global_init(CURL_GLOBAL_DEFAULT);
        }

        Client::~Client() {
            curl_global_cleanup();
        }

        std::vector<Book> Client::getHttps() {
            CURL *curl;
            CURLcode res;
            std::string readBuffer;

            std::string androidCacertPath = "/data/data/com.example.bookStore/cache/cacert.pem";

            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://www.googleapis.com/books/v1/volumes?q=android+ndk&maxResults=7&startIndex=0&projection=lite");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

                // Configurações adicionais para suportar HTTPS
                curl_easy_setopt(curl, CURLOPT_CAINFO, androidCacertPath.c_str());
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res == CURLE_OK) {
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "res == CURLE_OK");

                    // Imprimir ou processar os dados recebidos conforme necessário

                    // Salvar a resposta em um arquivo temporário
                    FILE* file = fopen("/data/data/com.example.bookStore/cache/response.txt", "w");
                    if (file) {
                        fprintf(file, "%s", readBuffer.c_str());
                        fclose(file);
                    } else {
                        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening file for writing");
                    }

                    // Analisa o JSON e obtém informações sobre os livros
                    std::vector<Book> books = parseBooksFromJson(readBuffer);
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "books");
                    printToLogcat(readBuffer);
                    return books;
                } else {
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "res == %d, error: %s", res, curl_easy_strerror(res));
                    // Tratar erro, se necessário
                    return {};
                }
            }
            return {};
        }

    }  // namespace http
}  // namespace curlssl