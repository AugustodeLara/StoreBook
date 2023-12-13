find_package(openssl REQUIRED CONFIG)

if(NOT TARGET curl::curl)
add_library(curl::curl SHARED IMPORTED)
set_target_properties(curl::curl PROPERTIES
    IMPORTED_LOCATION "/home/aglara/.gradle/caches/transforms-3/c225408ffd12ee8d5466fa04fc7734d2/transformed/jetified-curl-7.79.1-beta-1/prefab/modules/curl/libs/android.x86/libcurl.so"
    INTERFACE_INCLUDE_DIRECTORIES "/home/aglara/.gradle/caches/transforms-3/c225408ffd12ee8d5466fa04fc7734d2/transformed/jetified-curl-7.79.1-beta-1/prefab/modules/curl/include"
    INTERFACE_LINK_LIBRARIES "openssl::crypto;openssl::ssl"
)
endif()

