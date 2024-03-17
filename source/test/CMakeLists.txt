set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/bin)
include(GoogleTest)
add_subdirectory(abstract_factory)
add_subdirectory(clone_factory)
add_subdirectory(functor)
add_subdirectory(object_factory)
add_subdirectory(singleton)
add_subdirectory(smart_ptr)

