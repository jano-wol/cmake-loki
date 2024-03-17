set(target clone-factory-tests)

set(source
    TestCloneFactory.cpp
)

add_executable(${target} ${source})

target_link_libraries(${target}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_discover_tests(${target})

