set(MAIN_SRCS main.cpp)

add_executable(${PROJECT_NAME} ${MAIN_SRCS})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    pistache
    disc_delivery
)
