if(NOT TARGET jsoncpp::jsoncpp)
add_library(jsoncpp::jsoncpp SHARED IMPORTED)
set_target_properties(jsoncpp::jsoncpp PROPERTIES
    IMPORTED_LOCATION "/home/aglara/.gradle/caches/transforms-3/da8885faedad5e03fe3cd1a84133783f/transformed/jetified-jsoncpp-1.9.5-beta-1/prefab/modules/jsoncpp/libs/android.x86/libjsoncpp.so"
    INTERFACE_INCLUDE_DIRECTORIES "/home/aglara/.gradle/caches/transforms-3/da8885faedad5e03fe3cd1a84133783f/transformed/jetified-jsoncpp-1.9.5-beta-1/prefab/modules/jsoncpp/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

