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
        // Google Books API URL

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getGoogleBooksData %s", "");

        // Make sure to replace "YOUR_CACERT_PATH" with the actual path to your cacert.pem file
        http::Client client(
                "/home/aglara/AndroidStudioProjects/lojaLivros/app/src/main/assets/cacert.pem");
        std::string url = "https://www.googleapis.com/books/v1/volumes?q=android&maxResults=14&startIndex=0&projection=lite";

        auto books = client.getHttps(url);

        if (!books.empty()) {
            for (auto &book: books) {
                if (book.description.length() > 200) {
                    book.description = book.description.substr(0, 200) +
                                       " ...";  // Add "..." to indicate truncation
                }
            }

            // Log information for each book
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
            return {};  // Return an empty vector if there are no books
        }
    }

    extern "C" JNIEXPORT jobjectArray JNICALL
    Java_com_example_bookStore_ui_main_MainActivity_getApiGoogleBook(JNIEnv *env,
                                                                     jobject /* this */,
                                                                     jstring cacert_java) {
        const char *cacert = env->GetStringUTFChars(cacert_java, nullptr);

        // Call the new function for the Google Books API
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

            // Create an instance of the BookManager class
            BookManager bookManager;

            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX - getApiGoogleBook book.id: %s",
                                book.id.c_str());

            // Call the function to check if the ID is present in the CSV
            bool isIDPresent = bookManager.checkIDinCSV(book.id);

            if (isIDPresent) {
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook FAVORITE: %d", 0);
            } else {
                // The ID is not present in the CSV file
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook FAVORITE: %d", 1);
            }

            // Update the favorite information in the book object
            book.favorite = isIDPresent;
        }

        size_t size = books.size();
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "getApiGoogleBook size %zu", size);

        env->ReleaseStringUTFChars(cacert_java, cacert);

        // Check if there are returned books
        if (!books.empty()) {
            // Create an array of Java String objects
            jobjectArray result = env->NewObjectArray(size * 6, env->FindClass("java/lang/String"),
                                                      env->NewStringUTF(""));

            int index = 0;
            // Fill the array with the values of each book
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

            // Iterate over the elements of the array and print each string individually
            for (size_t i = 0; i < size * 6; ++i) {
                jstring str = (jstring) env->GetObjectArrayElement(result, i);
                const char *utfChars = env->GetStringUTFChars(str, nullptr);
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Iterate: %s", utfChars);
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

        // Create a Book object with the details of the favorited book
        Book favoriteBook(id, title, description, thumbnail, true, ""); //TODO: Add BuyLink

        // Create an instance of the BookManager class
        BookManager bookManager;

        // Call the function to save the book to the CSV file
        bookManager.saveBookToCSV(favoriteBook);

        // Release resources
        env->ReleaseStringUTFChars(jId, id);
        env->ReleaseStringUTFChars(jTitle, title);
        env->ReleaseStringUTFChars(jDescription, description);
        env->ReleaseStringUTFChars(jThumbnail, thumbnail);
    }

    extern "C" JNIEXPORT bool JNICALL
    Java_com_example_bookStore_ui_main_BookDetailActivity_unfollowBook(JNIEnv *env, jobject /* this */,
                                                                       jstring jId) {
        const char *id = env->GetStringUTFChars(jId, nullptr);
        // Create an instance of the BookManager class
        BookManager bookManager;
        // Check if the book is present in the CSV file
        if (bookManager.checkIDinCSV(id)) {
            // Open the original CSV file for reading
            std::ifstream csvReadFile(bookManager.csvFilePath);
            // Create a new temporary CSV file to store the books that should not be removed
            std::ofstream tempCsvWriteFile("/sdcard/Download/favorite_books_temp.csv",
                                           std::ios::app);
            if (csvReadFile.is_open() && tempCsvWriteFile.is_open()) {
                std::string line;
                while (std::getline(csvReadFile, line)) {
                    // Extract the ID from the line
                    std::istringstream iss(line);
                    std::string currentId;
                    if (std::getline(iss, currentId, ',')) {

                        currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'),
                                        currentId.end());

                        __android_log_print(ANDROID_LOG_VERBOSE, TAG,
                                            "XXXX - unfollowBook currentId: %s", currentId.c_str());
                        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX - unfollowBook id: %s",
                                            id);


                        // If the ID does not match the ID of the book to be removed, write the line to the new file
                        if (currentId != id) {
                            tempCsvWriteFile << line << "\n";
                        }
                    }
                }
                // Close the files
                csvReadFile.close();
                tempCsvWriteFile.close();
                // Replace the original file with the temporary one
                std::rename("/sdcard/Download/favorite_books_temp.csv",
                            bookManager.csvFilePath.c_str());
                // Release resources
                env->ReleaseStringUTFChars(jId, id);
                return true;
            } else {
                // If there is an error opening the files, return false
                env->ReleaseStringUTFChars(jId, id);
                return false;
            }
        } else {
            // If the ID is not present in the CSV file, return false
            env->ReleaseStringUTFChars(jId, id);
            return false;
        }
    }

    extern "C"
    JNIEXPORT jobjectArray JNICALL
    Java_com_example_bookStore_ui_favorites_FavoritesActivity_getFavoritesSaved(JNIEnv *env, jobject thiz) {
        // Create an instance of the BookManager class
        BookManager bookManager;

        // Get the list of favorite books from the CSV file
        auto favoriteBooks = bookManager.getFavoriteBooksFromCSV();

        // Check if there are returned books
        if (!favoriteBooks.empty()) {
            // Get the Java class for the String class
            jclass stringClass = env->FindClass("java/lang/String");

            // Create an array of Java String objects
            jobjectArray result = env->NewObjectArray(favoriteBooks.size(), stringClass, nullptr);

            // Fill the array with the values of each book
            for (size_t i = 0; i < favoriteBooks.size(); ++i) {
                const Book &book = favoriteBooks[i];
                std::string bookString = book.id + "**" +
                                         book.title + "**" +
                                         book.description + "**" +
                                         book.smallThumbnail + "**";

                // Check if the jstring object is not null
                jstring jBookString = env->NewStringUTF(bookString.c_str());
                if (jBookString != nullptr) {
                    // Set the object at position i in the array
                    env->SetObjectArrayElement(result, i, jBookString);

                    // Release the local reference
                    env->DeleteLocalRef(jBookString);
                } else {
                    // Handle error situation if needed
                    __android_log_print(ANDROID_LOG_ERROR, TAG,
                                        "Error creating jstring for book %zu", i);
                }
            }

            // Release the local reference of the String class
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

        // Call the function to check if the ID is present in the CSV file
        bool isIDPresent = bookManager.checkIDinCSV(id);

        return isIDPresent;
    }

    extern "C" JNIEXPORT bool JNICALL
    Java_com_example_bookStore_ui_main_MainActivity_hasSavedBook(JNIEnv *env, jobject /* this */) {
        BookManager bookManager;
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX hasSavedBoo");

        // Call the function to check if there are elements in the CSV file
        bool saved = bookManager.checkElementsCSV();

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXX hasSavedBoo %d", saved);

        return saved;
    }

}  // namespace curlssl
