set(SRC disc_delivery.cpp)
set(LIBRARY_NAME disc_delivery)

file(GLOB SRC *.cpp)

add_library(${LIBRARY_NAME} STATIC ${SRC})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
        ${LIBRARY_NAME} PRIVATE
        pistache

        disc_models
        disc_repo
)
