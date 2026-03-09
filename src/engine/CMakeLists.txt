add_library(mcse-engine
    order_book.cpp
    order_book.hpp
    price_level.hpp
)

target_include_directories(mcse-engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(mcse-engine PUBLIC mcse-protocol)
