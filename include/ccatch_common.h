#ifndef __CCATCH_COMMON_H__
#define __CCATCH_COMMON_H__

#ifndef __CCATCH_H__
#error "Do not include this file directly. Use ccatch.h or ccatch_with_main.h instead"
#endif

#if __STDC_VERSION__ < 199901L
#error "Only version greater C99 is supported"
#endif

#include <stdbool.h>
#include <stdint.h>

typedef void (ccatch_function_f)(int, int*, const char**);

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
