set(target singleton-tests)

set(sources
    TestSingleton.cpp
)

add_executable(${target} ${sources})

target_link_libraries(${target}
    PUBLIC
	loki
	PRIVATE
	gmock-dev
	GTest::gtest_main
)
gtest_discover_tests(${target})

