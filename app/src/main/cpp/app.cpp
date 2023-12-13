#include <jni.h>
#include <fstream>
#include <android/log.h>
#include "https.h"
#include "java_interop.h"
#include "json/json.h"
#include "logging.h"
#include "Book.h"
#include "BookManager.h"
#include <algorithm>
#include <set>


#define TAG "app"

namespace curlssl {

    std::vector<Book> getGoogleBooksData() {
        // URL da API do Google Books

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getGoogleBooksData %s", "");

        // Certifique-se de substituir "YOUR_CACERT_PATH" pelo caminho real para o seu arquivo cacert.pem
        http::Client client(
                "/home/aglara/AndroidStudioProjects/lojaLivros/app/src/main/assets/cacert.pem");
        std::string url = "https://www.googleapis.com/books/v1/volumes?q=android&maxResults=14&startIndex=0&projection=lite";

        auto books = client.getHttps(url);

        if (!books.empty()) {
            for (auto &book: books) {
                if (book.description.length() > 200) {
                    book.description = book.description.substr(0, 200) +
                                       " ...";  // Adiciona "..." para indicar que foi truncado
                }
            }

            // Exibir informações para cada livro no log
            for (const auto &book: books) {
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.id.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.title.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.description.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.smallThumbnail.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.favorite ? "Favorite" : "Not Favorite");
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "cURL Request Executed %s",
                                    book.buyLink.c_str());

            }

            return books;
        } else {
            return {};  // Retorna um vetor vazio se não houver livros
        }
    }


    extern "C" JNIEXPORT jobjectArray JNICALL
    Java_com_example_bookStore_ui_main_MainActivity_getApiGoogleBook(JNIEnv *env,
                                                             jobject /* this */,
                                                             jstring cacert_java) {
        const char *cacert = env->GetStringUTFChars(cacert_java, nullptr);

        // Chamar a nova função para a API do Google Books
        auto books = curlssl::getGoogleBooksData();

        for (auto &book: books) {
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook id: %s",
                                book.id.c_str());
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook title: %s",
                                book.title.c_str());
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook description: %s",
                                book.description.c_str());
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook image: %s",
                                book.smallThumbnail.c_str());

            // Criar uma instância da classe BookManager
            BookManager bookManager;

            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX - getApiGoogleBook book.id: %s",
                                book.id.c_str());


            // Chamar a função para verificar se o ID está presente no CSV
            bool isIDPresent = bookManager.checkIDinCSV(book.id);

            if (isIDPresent) {
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook FAVORITO: %d", 0);
            } else {
                // O ID não está presente no arquivo CSV
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook FAVORITO: %d", 1);
            }

            // Atualizar a informação de favorito no objeto book
            book.favorite = isIDPresent;

        }

        size_t size = books.size();
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook tamanho %zu", size);

        env->ReleaseStringUTFChars(cacert_java, cacert);

        // Verificar se há livros retornados
        if (!books.empty()) {
            // Criar um array de objetos String em Java
            jobjectArray result = env->NewObjectArray(size * 6, env->FindClass("java/lang/String"),
                                                      env->NewStringUTF(""));

            int index = 0;
            // Preencher o array com os valores de cada livro
            for (const auto &book: books) {
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF((book.id + "**").c_str()));
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF((book.title + "**").c_str()));
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF((book.description + "**").c_str()));
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF((book.smallThumbnail + "**").c_str()));
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF(
                                                   (std::to_string(book.favorite ? 1 : 0) +
                                                    "**").c_str()));
                env->SetObjectArrayElement(result, index++,
                                           env->NewStringUTF((book.buyLink + "**").c_str()));
            }

            // Iterar sobre os elementos do array e imprimir cada string individualmente
            for (size_t i = 0; i < size * 6; ++i) {
                jstring str = (jstring) env->GetObjectArrayElement(result, i);
                const char *utfChars = env->GetStringUTFChars(str, nullptr);
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Iterar: %s", utfChars);
                env->ReleaseStringUTFChars(str, utfChars);
                env->DeleteLocalRef(str);
            }

            return result;
        } else {
            return nullptr;
        }
    }

    extern "C" JNIEXPORT void JNICALL
    Java_com_example_bookStore_ui_main_BookDetailActivity_favoriteBook(JNIEnv *env, jobject /* this */,
                                                               jstring jId, jstring jTitle,
                                                               jstring jDescription,
                                                               jstring jThumbnail) {
        const char *id = env->GetStringUTFChars(jId, nullptr);
        const char *title = env->GetStringUTFChars(jTitle, nullptr);
        const char *description = env->GetStringUTFChars(jDescription, nullptr);
        const char *thumbnail = env->GetStringUTFChars(jThumbnail, nullptr);

        // Criar um objeto Book com os detalhes do livro favoritado
        Book favoriteBook(id, title, description, thumbnail, true, ""); //TODO: Adicionar BuyLink

        // Criar uma instância da classe BookManager
        BookManager bookManager;

        // Chamar a função para salvar o livro no arquivo CSV
        bookManager.saveBookToCSV(favoriteBook);

        // Liberar recursos
        env->ReleaseStringUTFChars(jId, id);
        env->ReleaseStringUTFChars(jTitle, title);
        env->ReleaseStringUTFChars(jDescription, description);
        env->ReleaseStringUTFChars(jThumbnail, thumbnail);
    }

    extern "C" JNIEXPORT bool JNICALL
    Java_com_example_bookStore_ui_main_BookDetailActivity_unfollowBook(JNIEnv *env, jobject /* this */,
                                                               jstring jId) {
        const char *id = env->GetStringUTFChars(jId, nullptr);
        // Criar uma instância da classe BookManager
        BookManager bookManager;
        // Verificar se o livro está presente no arquivo CSV
        if (bookManager.checkIDinCSV(id)) {
            // Abrir o arquivo CSV original para leitura
            std::ifstream csvReadFile(bookManager.csvFilePath);
            // Criar um novo arquivo CSV temporário para armazenar os livros que não devem ser removidos
            std::ofstream tempCsvWriteFile("/sdcard/Download/favorite_books_temp.csv",
                                           std::ios::app);
            if (csvReadFile.is_open() && tempCsvWriteFile.is_open()) {
                std::string line;
                while (std::getline(csvReadFile, line)) {
                    // Extrai a ID da linha
                    std::istringstream iss(line);
                    std::string currentId;
                    if (std::getline(iss, currentId, ',')) {

                        currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'),
                                        currentId.end());

                        __android_log_print(ANDROID_LOG_VERBOSE, TAG,
                                            "XXXX - unfollowBook currentId: %s", currentId.c_str());
                        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX - unfollowBook id: %s",
                                            id);


                        // Se a ID não corresponder à ID do livro a ser removido, escreva a linha no novo arquivo
                        if (currentId != id) {
                            tempCsvWriteFile << line << "\n";
                        }
                    }
                }
                // Fechar os arquivos
                csvReadFile.close();
                tempCsvWriteFile.close();
                // Substituir o arquivo original pelo temporário
                std::rename("/sdcard/Download/favorite_books_temp.csv",
                            bookManager.csvFilePath.c_str());
                // Liberar recursos
                env->ReleaseStringUTFChars(jId, id);
                return true;
            } else {
                // Se houver erro ao abrir os arquivos, retornar false
                env->ReleaseStringUTFChars(jId, id);
                return false;
            }
        } else {
            // Se a ID não estiver presente no arquivo CSV, retornar false
            env->ReleaseStringUTFChars(jId, id);
            return false;
        }
    }

    extern "C"
    JNIEXPORT jobjectArray JNICALL
    Java_com_example_bookStore_ui_favorites_FavoritesActivity_getFavoritesSaved(JNIEnv *env, jobject thiz) {
        // Criar uma instância da classe BookManager
        BookManager bookManager;

        // Obter a lista de livros favoritos do arquivo CSV
        auto favoriteBooks = bookManager.getFavoriteBooksFromCSV();

        // Verificar se há livros retornados
        if (!favoriteBooks.empty()) {
            // Obter a classe Java para a classe String
            jclass stringClass = env->FindClass("java/lang/String");

            // Criar um array de objetos Java String
            jobjectArray result = env->NewObjectArray(favoriteBooks.size(), stringClass, nullptr);

            // Preencher o array com os valores de cada livro
            for (size_t i = 0; i < favoriteBooks.size(); ++i) {
                const Book &book = favoriteBooks[i];
                std::string bookString = book.id + "**" +
                                         book.title + "**" +
                                         book.description + "**" +
                                         book.smallThumbnail + "**";

                // Verificar se o objeto jstring não é nulo
                jstring jBookString = env->NewStringUTF(bookString.c_str());
                if (jBookString != nullptr) {
                    // Definir o objeto na posição i do array
                    env->SetObjectArrayElement(result, i, jBookString);

                    // Liberar a referência local
                    env->DeleteLocalRef(jBookString);
                } else {
                    // Lida com a situação de erro, se necessário
                    __android_log_print(ANDROID_LOG_ERROR, TAG,
                                        "Erro ao criar jstring para o livro %zu", i);
                }
            }

            // Liberar a referência local da classe String
            env->DeleteLocalRef(stringClass);

            return result;
        } else {
            return nullptr;
        }
    }

    extern "C" JNIEXPORT bool JNICALL
    Java_com_example_bookStore_ui_main_BookDetailActivity_isFavorite(JNIEnv *env, jobject /* this */,
                                                             jstring jId) {
        const char *id = env->GetStringUTFChars(jId, nullptr);
        BookManager bookManager;
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX - isFavorite: %s", id);

        // Chamar a função para verificar se o ID está presente no CSV
        bool isIDPresent = bookManager.checkIDinCSV(id);


        return isIDPresent;
    }


    extern "C" JNIEXPORT bool JNICALL
    Java_com_example_bookStore_ui_main_MainActivity_hasSavedBook(JNIEnv *env, jobject /* this */) {
        BookManager bookManager;
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX hasSavedBoo");

        // Chamar a função para verificar se o ID está presente no CSV
        bool saved = bookManager.checkElementsCSV();

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX hasSavedBoo %d", saved);


        return saved;
    }


}  // namespace curlssl
