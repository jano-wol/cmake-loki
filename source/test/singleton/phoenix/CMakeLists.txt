set(target singleton-phoenix-tests)

set(source
    TestSingletonPhoenix.cpp
)

add_executable(${target} ${source})

target_link_libraries(${target}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_add_tests(TARGET ${target})

