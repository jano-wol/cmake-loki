set(target object-factory-tests)

set(source
    TestObjectFactory.cpp
)

add_executable(${target} ${source})

target_link_libraries(${target}
	PRIVATE
	gmock-dev
	GTest::gtest_main
	loki
)
gtest_add_tests(TARGET ${target})

