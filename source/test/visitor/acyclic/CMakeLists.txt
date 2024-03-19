set(target visitor-acyclic-tests)

set(source
    TestAcyclicVisitor.cpp
)

add_executable(${target} ${source})

target_link_libraries(${target}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_discover_tests(${target})

