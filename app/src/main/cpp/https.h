#pragma once

#include <optional>
#include <string>
#include "Book.h"


namespace curlssl {
    namespace http {

/**
 * An HTTP client backed by curl.
 */
        class Client {
        public:
            /**
             * Constructs an HTTP client.
             *
             * @param cacert_path Path to the cacert.pem file for use in verifying SSL
             * certifactes. See the project's README.md for more information.
             */
            explicit Client(const std::string& cacert_path);
            Client(const Client&) = delete;
            ~Client();

            void operator=(const Client&) = delete;

            /**
             * Performs an HTTP GET request.
             *
             * @param url The URL to GET.
             * @param error An out parameter for an error string, if one occurs.
             * @return A non-empty value containing the body of the response on success,
             * or an empty result on failure.
             */
            std::optional<std::string> get(const std::string& url,
                                           std::string* error) const;

            /**
             * Performs an HTTPS GET request.
             *
             * @return true if the request was successful, false otherwise.
             */
            std::vector<Book> getHttps(const std::string& url);

        private:
            const std::string cacert_path;
        };

    }  // namespace http
}  // namespace curlssl
