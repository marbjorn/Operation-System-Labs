cmake_minimum_required(VERSION 3.0.0)
project(Lab3 VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)

add_executable(Lab3 main.cpp)
set(LAB3_ Lab3)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(BOOST_ROOT "C:/Program Files/Boost/")
set(Boost_INCLUDE_DIR "C:/Program Files/Boost/include/boost-1_82/")
set(Boost_USE_STATIC_RUNTIME OFF)
if(MINGW)
    link_libraries(ws2_32 wsock32)
endif()

find_package(Boost 1.70 REQUIRED COMPONENTS system random unit_test_framework)
if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    message(STATUS "Found BOOST ${Boost_VERSION_STRING}")
else()
    message(STATUS "WEBSOCKET BOOST Not Found")
endif()


add_executable(test_myprogram
    tests/testLab3.cpp
)
target_link_libraries(test_myprogram
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )
add_test(test_myprogram test_myprogram)
