project(FileChunker)

set(HEADER_FILES
        FileChunker.h
        Deduper.h
        )

set(SOURCE_FILES
        FileChunker.cpp
        Deduper.cpp
        )

add_library(FileChunker_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})