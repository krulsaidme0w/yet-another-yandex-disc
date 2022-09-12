find_package(nlohmann_json 3.2.0 REQUIRED)

set(SRC disc_models.cpp)
set(LIBRARY_NAME disc_models)

file(GLOB SRC *.cpp)

add_library(${LIBRARY_NAME} STATIC ${SRC})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
        ${LIBRARY_NAME} PRIVATE
        nlohmann_json::nlohmann_json
)
