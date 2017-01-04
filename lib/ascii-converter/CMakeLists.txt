
add_library(ascii-converter
        AsciiConverter.cpp
        ../../include/ascii-converter/AsciiConverter.hpp
        )

set_target_properties(ascii-converter
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS ascii-converter
        ARCHIVE DESTINATION lib
        )

