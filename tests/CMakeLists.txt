################################################################################
# Macros
################################################################################

macro(ProvideCatch2)

    set(BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 test")

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 2ebc04190310ca741ff234be78701e6b6fa0a95f
    )
    FetchContent_MakeAvailable(Catch2)

endmacro()

macro(CreateTests)

    add_executable(jungles_ring_buffer_test test_ring_buffer.cpp)
    target_link_libraries(jungles_ring_buffer_test PRIVATE Catch2::Catch2WithMain jungles::ring_buffer)
    target_compile_options(jungles_ring_buffer_test PRIVATE -Wall -Wextra)
    add_test(NAME test_jungles_ring_buffer COMMAND valgrind --leak-check=full $<TARGET_FILE:jungles_ring_buffer_test>)

endmacro()

function(MakeStaticAssertionTest target pass_regex)

    target_link_libraries(${target} PRIVATE jungles::ring_buffer)
    set_target_properties(${target} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    set(test_name test_${target})
    add_test(NAME ${test_name} COMMAND ${CMAKE_COMMAND} --build . --target ${target})
    set_tests_properties(${test_name} PROPERTIES PASS_REGULAR_EXPRESSION ${pass_regex})

endfunction()

macro(CreateCompileTimeTests)

    add_executable(jungles_ring_buffer_must_be_default_constructible
        test_ring_buffer_with_non_default_constructible.cpp)
    MakeStaticAssertionTest(jungles_ring_buffer_must_be_default_constructible ".*constraints not satisfied.*DefaultConstructible.*")

    add_executable(jungles_ring_buffer_size_must_be_power_of_two
        test_ring_buffer_with_non_power_of_two.cpp)
    MakeStaticAssertionTest(jungles_ring_buffer_size_must_be_power_of_two ".*constraints not satisfied.*PowerOfTwo.*")
    # Must be power of two size

endmacro()

################################################################################
# Main script
################################################################################

ProvideCatch2()
CreateTests()
CreateCompileTimeTests()
