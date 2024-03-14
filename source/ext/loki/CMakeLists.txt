set(target loki)
set(library_path "${CMAKE_CURRENT_SOURCE_DIR}/${target}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUCHAR_MAX=255")
set(headers ${library_path}/AbstractFactory.h
            ${library_path}/AssocVector.h
            ${library_path}/DataGenerators.h
            ${library_path}/EmptyType.h
            ${library_path}/Factory.h
            ${library_path}/Functor.h
            ${library_path}/HierarchyGenerators.h
            ${library_path}/LokiTypeInfo.h
            ${library_path}/MultiMethods.h
            ${library_path}/NullType.h
            ${library_path}/Singleton.h
            ${library_path}/SmallObj.h
            ${library_path}/SmartPtr.h
            ${library_path}/static_check.h
            ${library_path}/Threads.h
            ${library_path}/Tuple.h
            ${library_path}/Typelist.h
            ${library_path}/TypeManip.h
            ${library_path}/TypeTraits.h
            ${library_path}/Visitor.h
            )
set(sources ${library_path}/Singleton.cpp
            ${library_path}/SmallObj.cpp
            )
add_library(${target} ${sources} ${headers})
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

