#ifndef __CCATCH_COMMON_H__
#define __CCATCH_COMMON_H__

#ifndef __CCATCH_H__
#error "Do not include this file directly. Use ccatch.h or ccatch_with_main.h instead"
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct ccatch_scope_t ccatch_scope_t;
typedef void (ccatch_scope_destructor_f)(void*);

void ccatch_add_object_to_scope(ccatch_scope_t* scope, void* object, ccatch_scope_destructor_f* destructor);

typedef void (ccatch_function_f)(int, int*, const char**, ccatch_scope_t*);

typedef struct ccatch_register_t
{
    uint32_t* global_object;
    uint32_t magic_number;
    const char* name;
    const char* tag;
    ccatch_function_f* function;
} ccatch_register_t;

#define CCATCH_MAGIC_NUMBER 0x00CCA7C4

extern uint32_t ccatch_global_object_g;
extern bool ccatch_test_failed_g;

#endif // __CCATCH_COMMON_H__
