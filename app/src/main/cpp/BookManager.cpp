// BookManager.cpp
#include "BookManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <android/log.h>
#include "logging.h"  // Make sure to include the header for logging

#define TAG "app"

BookManager::BookManager() {
    // Path to the internal CSV file on the device
    csvFilePath = "/sdcard/Download/favorite_books.csv";
}

void BookManager::saveBookToCSV(const Book& book) {
    // Open the CSV file for reading
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Using a set to store unique IDs
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extract the ID from the line
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {
                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());

                // Display the ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Existing ID in CSV: %s", currentId.c_str());
                // Add the ID to the set
                existingIds.insert(currentId);
            }
        }
        // Close the reading file
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening CSV file for reading");
    }
    // Check if the ID already exists in the set
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXXX saveBookToCSV ID=  %s", book.id.c_str());

    if (existingIds.find(book.id) != existingIds.end()) {
        // The ID already exists, no need to add it again
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Book with ID %s already exists in CSV", book.id.c_str());
        return;
    }
    // Open the CSV file to add new data
    std::ofstream csvWriteFile(csvFilePath, std::ios::app);
    if (csvWriteFile.is_open()) {
        // Check if the ID already exists in the set
        if (existingIds.find(book.id) != existingIds.end()) {
            // The ID already exists, no need to add it again
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Book with ID %s already exists in CSV", book.id.c_str());
        } else{
            __android_log_print(ANDROID_LOG_VERBOSE, TAG, "New book %s in CSV", book.id.c_str());
            csvWriteFile << book.id + "|" << "," << book.title + "|" << "," << book.description  + "|" << "," << book.smallThumbnail << + "|" "\n";
        }
        // Close the file
        csvWriteFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening CSV file for writing");
    }
}

std::vector<Book> BookManager::getFavoriteBooksFromCSV() {
    std::vector<Book> favoriteBooks;

    // Open the CSV file for reading
    std::ifstream csvReadFile(csvFilePath);

    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Find the positions of pipes (|)
            size_t idPos = 0;
            size_t titlePos = line.find('|', idPos);
            size_t descriptionPos = line.find('|', titlePos + 1);
            size_t imagePos = line.find('|', descriptionPos + 1);

            // Check if the positions are valid
            if (titlePos != std::string::npos && descriptionPos != std::string::npos && imagePos != std::string::npos) {
                // Extract the book details from the line
                std::string id = line.substr(idPos, titlePos - idPos);
                std::string title = line.substr(titlePos + 1, descriptionPos - titlePos - 1);
                std::string description = line.substr(descriptionPos + 1, imagePos - descriptionPos - 1);
                std::string image = line.substr(imagePos + 1);

                // Remove unwanted commas
                title.erase(std::remove(title.begin(), title.end(), ','), title.end());
                description.erase(std::remove(description.begin(), description.end(), ','), description.end());
                image.erase(std::remove(image.begin(), image.end(), ','), image.end());

                // Remove "|" character at the end of the image
                if (!image.empty() && image[image.length() - 1] == '|') {
                    image.pop_back();
                }

                // Log to verify
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", id.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", title.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", description.c_str());
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "XXXXX getFavoriteBooksFromCSV %s", image.c_str());

                // Create a Book instance with a suitable constructor and add it to the favorites list
                Book bk(id.c_str(), title.c_str(), description.c_str(), image.c_str());
                favoriteBooks.push_back(bk);


            }
        }

        // Close the reading file
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening CSV file for reading");
    }

    return favoriteBooks;
}

bool BookManager::checkIDinCSV(std::string id) {
    // Open the CSV file for reading
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Using a set to store unique IDs
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extract the ID from the line
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {

                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());

                // Display the ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Existing ID in CSV: %s", currentId.c_str());
                // Add the ID to the set
                existingIds.insert(currentId);
            }
        }
        // Close the reading file
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening CSV file for reading");
        return false; // If there is an error opening the file, consider it as a non-existing ID
    }
    // Check if the ID already exists in the set
    return existingIds.find(id) != existingIds.end();
}

bool BookManager::checkElementsCSV() {
    // Open the CSV file for reading
    std::ifstream csvReadFile(csvFilePath);
    std::set<std::string> existingIds; // Using a set to store unique IDs
    if (csvReadFile.is_open()) {
        std::string line;
        while (std::getline(csvReadFile, line)) {
            // Extract the ID from the line
            std::istringstream iss(line);
            std::string currentId;
            if (std::getline(iss, currentId, ',')) {

                currentId.erase(std::remove(currentId.begin(), currentId.end(), '|'), currentId.end());
                // Display the ID
                __android_log_print(ANDROID_LOG_VERBOSE, TAG, "checkElementsCSV Existing ID in CSV: %s", currentId.c_str());
                if (!currentId.empty()) {
                    // If at least one non-empty ID is found, return true
                    csvReadFile.close();
                    return true;
                }
            }
        }
        // Close the reading file
        csvReadFile.close();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Error opening CSV file for reading");
        return false; // If there is an error opening the file, consider it as a non-existing ID
    }

    // If no non-empty ID was found, return false
    return false;
}
