find_package(libpqxx REQUIRED)

set(MAIN_SRCS main.cpp)

add_executable(${PROJECT_NAME} ${MAIN_SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    pistache
    libpqxx::pqxx

    disc_delivery
    disc_models
    disc_repo
)
