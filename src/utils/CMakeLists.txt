set(SOURCES
    utils.h
    requests.h
    )

add_library(utils
    ${SOURCES}
    )

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
