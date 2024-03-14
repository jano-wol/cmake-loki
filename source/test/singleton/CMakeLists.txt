set(target-cration-policy singleton-tests-creation-policy)

set(source-cration-policy
    TestSingletonCreationPolicy.cpp
)

add_executable(${target-cration-policy} ${source-cration-policy})

target_link_libraries(${target-cration-policy}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_discover_tests(${target-cration-policy})

