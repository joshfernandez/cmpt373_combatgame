
add_library(networking
        server.cpp
        client.cpp
        )

set_target_properties(networking
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS networking
        ARCHIVE DESTINATION lib
        )

