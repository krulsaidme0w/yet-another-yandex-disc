find_package(libpqxx REQUIRED)

set(SRC disc_repo.cpp)
set(LIBRARY_NAME disc_repo)

file(GLOB SRC *.cpp)

add_library(${LIBRARY_NAME} STATIC ${SRC})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

target_link_libraries(
        ${LIBRARY_NAME} PRIVATE
        
        libpqxx::pqxx

        disc_models
)
