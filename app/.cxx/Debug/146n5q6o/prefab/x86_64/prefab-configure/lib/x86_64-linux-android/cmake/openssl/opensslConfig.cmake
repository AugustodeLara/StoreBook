if(NOT TARGET openssl::crypto)
add_library(openssl::crypto SHARED IMPORTED)
set_target_properties(openssl::crypto PROPERTIES
    IMPORTED_LOCATION "/home/aglara/.gradle/caches/transforms-3/77151bcfb2f5bbfdc9f1b26aea524e73/transformed/jetified-openssl-1.1.1l-beta-1/prefab/modules/crypto/libs/android.x86_64/libcrypto.so"
    INTERFACE_INCLUDE_DIRECTORIES "/home/aglara/.gradle/caches/transforms-3/77151bcfb2f5bbfdc9f1b26aea524e73/transformed/jetified-openssl-1.1.1l-beta-1/prefab/modules/crypto/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

if(NOT TARGET openssl::ssl)
add_library(openssl::ssl SHARED IMPORTED)
set_target_properties(openssl::ssl PROPERTIES
    IMPORTED_LOCATION "/home/aglara/.gradle/caches/transforms-3/77151bcfb2f5bbfdc9f1b26aea524e73/transformed/jetified-openssl-1.1.1l-beta-1/prefab/modules/ssl/libs/android.x86_64/libssl.so"
    INTERFACE_INCLUDE_DIRECTORIES "/home/aglara/.gradle/caches/transforms-3/77151bcfb2f5bbfdc9f1b26aea524e73/transformed/jetified-openssl-1.1.1l-beta-1/prefab/modules/ssl/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

