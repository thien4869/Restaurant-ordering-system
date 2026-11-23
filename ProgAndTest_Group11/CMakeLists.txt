cmake_minimum_required(VERSION 3.18)
project(RestaurantOrderGroup11 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

# Preserve original ZIPs; two independent console prototypes, not an integrated app.
set(ACCOUNT_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/submissions/tv3/TV3_Account_Submission.zip")
set(ORDERS_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/submissions/tv3/TV3_CPP.zip")
foreach(ARCHIVE IN ITEMS "${ACCOUNT_ARCHIVE}" "${ORDERS_ARCHIVE}")
  if(NOT EXISTS "${ARCHIVE}")
    message(FATAL_ERROR "Missing team submission: ${ARCHIVE}")
  endif()
endforeach()

set(ACCOUNT_EXTRACT "${CMAKE_CURRENT_BINARY_DIR}/extracted/account")
set(ORDERS_EXTRACT "${CMAKE_CURRENT_BINARY_DIR}/extracted/orders")
file(MAKE_DIRECTORY "${ACCOUNT_EXTRACT}" "${ORDERS_EXTRACT}")
file(ARCHIVE_EXTRACT INPUT "${ACCOUNT_ARCHIVE}" DESTINATION "${ACCOUNT_EXTRACT}")
file(ARCHIVE_EXTRACT INPUT "${ORDERS_ARCHIVE}" DESTINATION "${ORDERS_EXTRACT}")
set(ACCOUNT_DIR "${ACCOUNT_EXTRACT}/TV3_Account_Submission")
set(ORDERS_DIR "${ORDERS_EXTRACT}/TV3_CPP")

add_library(tv3_account_lib STATIC
  "${ACCOUNT_DIR}/src/AccountService.cpp"
  "${ACCOUNT_DIR}/src/UserRepository.cpp")
target_include_directories(tv3_account_lib PUBLIC "${ACCOUNT_DIR}/src")
add_executable(tv3_account_demo "${ACCOUNT_DIR}/src/main.cpp")
target_link_libraries(tv3_account_demo PRIVATE tv3_account_lib)
add_executable(tv3_account_tests "${ACCOUNT_DIR}/tests/tests_unit.cpp")
target_link_libraries(tv3_account_tests PRIVATE tv3_account_lib)
add_test(NAME account_checks COMMAND tv3_account_tests)

add_library(tv3_orders_lib STATIC
  "${ORDERS_DIR}/Order.cpp"
  "${ORDERS_DIR}/OrderRepository.cpp"
  "${ORDERS_DIR}/OrderService.cpp")
target_include_directories(tv3_orders_lib PUBLIC "${ORDERS_DIR}")
add_executable(tv3_orders_demo "${ORDERS_DIR}/main.cpp")
target_link_libraries(tv3_orders_demo PRIVATE tv3_orders_lib)
add_executable(tv3_orders_tests "${ORDERS_DIR}/tests_unit.cpp")
target_link_libraries(tv3_orders_tests PRIVATE tv3_orders_lib)
add_test(NAME orders_checks COMMAND tv3_orders_tests)

message(STATUS "Independent TV3 C++ prototypes configured; no combined app yet")

# ==========================================
# TV4 - Orders Module
# ==========================================

add_library(tv4_orders_lib STATIC
    src/repositories/OrderRepository.cpp
    src/services/OrderService.cpp
)
target_include_directories(tv4_orders_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_executable(tv4_orders_tests
    tests/test_orders.cpp
)
target_link_libraries(tv4_orders_tests PRIVATE
    tv4_orders_lib
)
add_test(
    NAME tv4_orders_checks
    COMMAND tv4_orders_tests
)

#TV5
add_library(tv5_users_lib STATIC
    src/repositories/UserRepository.cpp
    src/services/UserAdminService.cpp
)
target_include_directories(tv5_users_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
set(TV5_TESTS
    test_users_list
    test_users_update
    test_users_delete
    test_users_search
    test_users_pagination
    test_users_admin
)
foreach(TEST_NAME IN LISTS TV5_TESTS)
    add_executable(${TEST_NAME}
        "tests/unit/${TEST_NAME}.cpp"
    )
    target_link_libraries(${TEST_NAME} PRIVATE
        tv5_users_lib
    )
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
endforeach()

