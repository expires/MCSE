file(GLOB MCSE_SOURCES "*.cpp")
file(GLOB MCSE_HEADERS "*.hpp")

add_library(mcse-engine
    ${MCSE_SOURCES}
    ${MCSE_HEADERS}
)

target_include_directories(mcse-engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
