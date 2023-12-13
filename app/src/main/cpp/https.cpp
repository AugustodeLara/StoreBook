#include "https.h"

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

#define TAG "HTTPS"

using namespace std::string_literals;

namespace {

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        size_t totalSize = size * nmemb;
        std::string* buffer = static_cast<std::string*>(userp);

        // Checks if the buffer is empty or if the last character is not a double quote
        if (buffer->empty() || buffer->back() != '"') {
            buffer->append(static_cast<char*>(contents), totalSize);
        } else {
        // If the buffer ended with a double quote, it means the description is being trimmed
            // In this case, we add a single quote at the beginning to continue reading the description
            buffer->pop_back();  // Remove a última aspas dupla
            buffer->push_back('\'');  // Adiciona uma aspas simples
            buffer->append(static_cast<char*>(contents), totalSize);
        }

        return totalSize;
    }

    // Split the string into smaller parts and print to logcat
    void printToLogcat(const std::string& buffer) {
        //__android_log_print(ANDROID_LOG_VERBOSE, TAG, "printToLogcat tamanho %d",buffer.length());

        const size_t chunkSize = 4000;  // Tamanho máximo permitido pelo logcat
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
            // Checks if the JSON object contains the "items" key
            if (root.isMember("items")) {
                // Obtém o array de livros
                const auto& items = root["items"];

                // Iterates over the books in the array
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
                        book.buyLink = "Unavailable";
                        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX BuyLink unavailable");
                    }

                    // Add the book to the vector
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

        std::vector<Book> Client::getHttps(const std::string& url) {
            CURL *curl;
            CURLcode res;
            std::string readBuffer;

            std::string androidCacertPath = "/data/data/com.example.bookStore/cache/cacert.pem";

            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://www.googleapis.com/books/v1/volumes?q=android&maxResults=14&startIndex=0&projection=lite");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

                curl_easy_setopt(curl, CURLOPT_CAINFO, androidCacertPath.c_str());
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res == CURLE_OK) {
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "res == CURLE_OK");


                    // Save the response to a temporary file
                    FILE* file = fopen("/data/data/com.example.bookStore/cache/response.txt", "w");
                    if (file) {
                        fprintf(file, "%s", readBuffer.c_str());
                        fclose(file);
                    } else {
                        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening file for writing");
                    }

                    // Parse JSON and get information about books
                    std::vector<Book> books = parseBooksFromJson(readBuffer);
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "books");
                    printToLogcat(readBuffer);
                    return books;
                } else {
                    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "res == %d, error: %s", res, curl_easy_strerror(res));
                    // Handle error if necessary
                    return {};
                }
            }
            return {};
        }

    }  // namespace http
}  // namespace curlssl