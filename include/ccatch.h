#ifndef __CCATCH_H__
#define __CCATCH_H__

#include "ccatch_common.h"

#include <stdio.h>

#define __CONCAT_NAME(PREFIX, LINENO) PREFIX##LINENO

#define __UNIQUE_NAME(PREFIX, LINENO) __CONCAT_NAME(PREFIX, LINENO)

#define __COUNT_ARGS_N(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, ...) n16
#define __COUNT_ARGS(...) __COUNT_ARGS_N(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __REDIRECT_CALL(FUNC, ...) FUNC(__VA_ARGS__)

#define __TEST_CASE(NAME, TAG, FUNCTION) __TEST_CASE2(NAME, TAG, FUNCTION)
#define __TEST_CASE2(NAME, TAG, FUNCTION) \
    static ccatch_function_f FUNCTION; \
    ccatch_register_t ccatch_register_##FUNCTION = { \
        .global_object = &ccatch_global_object_g, \
        .magic_number = CCATCH_MAGIC_NUMBER, \
        .name = NAME, \
        .tag = TAG, \
        .function = FUNCTION \
    }; \
    static void FUNCTION(int __ccatch_index, int* __ccatch_count, const char** __ccatch_section_name, ccatch_scope_t* __ccatch_scope)

#define TEST_CASE(...) __REDIRECT_CALL(__REDIRECT_CALL(__CONCAT_NAME, TEST_CASE_, __COUNT_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define TEST_CASE_1(NAME) __TEST_CASE(NAME, "[default]", __UNIQUE_NAME(ccatch_test_case_, __LINE__))
#define TEST_CASE_2(NAME, TAG) __TEST_CASE(NAME, TAG, __UNIQUE_NAME(ccatch_test_case_, __LINE__))

#define SECTION(NAME) \
    if (__ccatch_index == (*__ccatch_count)++ && (*__ccatch_section_name = NAME))

#define MESSAGE(SEVERITY, MSG) printf("%s: %s at %s:%u\n", SEVERITY, MSG, __FILE__, __LINE__);

#define __REQUIRE(CONDITION, ACTION) \
    if (!(CONDITION)) \
    { \
        MESSAGE("ERROR", #CONDITION); \
        ccatch_test_failed_g = true; \
        ACTION \
    } \

#define REQUIRE(CONDITION) __REQUIRE(CONDITION, return;)
#define REQUIRE_EXIT(CONDITION, VALUE) __REQUIRE(CONDITION, return (VALUE);)
#define CHECK(CONDITION) __REQUIRE(CONDITION, ;)

#define SCOPE_ADD(...) __REDIRECT_CALL(__REDIRECT_CALL(__CONCAT_NAME, SCOPE_ADD_, __COUNT_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define SCOPE_ADD_2(OBJ, DEINIT) ccatch_add_object_to_scope(__ccatch_scope, (void*)(OBJ), (ccatch_scope_destructor_f*)(DEINIT));
#define SCOPE_ADD_4(O0, D0, O1, D1) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_2(O1, D1); }
#define SCOPE_ADD_6(O0, D0, O1, D1, O2, D2) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_4(O1, D1, O2, D2); }
#define SCOPE_ADD_8(O0, D0, O1, D1, O2, D2, O3, D3) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_6(O1, D1, O2, D2, O3, D3); }
#define SCOPE_ADD_10(O0, D0, O1, D1, O2, D2, O3, D3, O4, D4) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_8(O1, D1, O2, D2, O3, D3, O4, D4); }
#define SCOPE_ADD_12(O0, D0, O1, D1, O2, D2, O3, D3, O4, D4, O5, D5) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_10(O1, D1, O2, D2, O3, D3, O4, D4, O5, D5); }
#define SCOPE_ADD_14(O0, D0, O1, D1, O2, D2, O3, D3, O4, D4, O5, D5, O6, D6) { SCOPE_ADD_2(O0, D0); SCOPE_ADD_12(O1, D1, O2, D2, O3, D3, O4, D4, O5, D5, O6, D6); }

#endif // __CCATCH_H__
