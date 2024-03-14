set(target singleton-tests-creation-policy)

set(source
    TestSingletonCreationPolicy.cpp
)

add_executable(${target} ${source})

target_link_libraries(${target}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_discover_tests(${target})

