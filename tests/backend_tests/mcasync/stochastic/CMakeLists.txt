
add_executable(StochasticTest StochasticTest.cpp)

target_link_libraries(
    StochasticTest
    gtest_main
    interflop-mcasync
    interflop-dummy-core
)

include(GoogleTest)
gtest_discover_tests(StochasticTest)