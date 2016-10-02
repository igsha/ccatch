#ifndef __CCATCH_WITH_MAIN_H__
#define __CCATCH_WITH_MAIN_H__

#include "ccatch.h"

#include <stdlib.h>
#include <string.h>

typedef struct ccatch_register_t
{
    const char* name;
    const char* tag;
    ccatch_function_f* function;
} ccatch_register_t;

bool ccatch_test_failed_g = false;

static size_t ccatch_test_cases_count = 0;
static ccatch_register_t* ccatch_registered_test_cases = NULL;

static bool ccatch_irrelevant_state = false;

void ccatch_add_test_case(const char* name, const char* tag, ccatch_function_f* function)
{
    if (function == NULL || ccatch_irrelevant_state)
        return;

    static size_t ccatch_test_cases_capacity = 0;

    if (ccatch_test_cases_count >= ccatch_test_cases_capacity)
    {
        if (ccatch_test_cases_capacity == 0)
            ccatch_test_cases_capacity = 10;
        else
            ccatch_test_cases_capacity *= 2;

        ccatch_register_t* temp = realloc(ccatch_registered_test_cases, ccatch_test_cases_capacity * sizeof(*ccatch_registered_test_cases));
        if (!temp)
        {
            free(ccatch_registered_test_cases);
            ccatch_irrelevant_state = true;
        }
        ccatch_registered_test_cases = temp;
    }

    ccatch_register_t* test_case = &ccatch_registered_test_cases[ccatch_test_cases_count++];
    test_case->name = name;
    test_case->tag = tag;
    test_case->function = function;
#if CCATCH_VERBOSE
    printf("INIT: adds test %s, [%s], %p\n", name, tag, *(void**)&test_case->function);
#endif
}

static int ccatch_run_tests(ccatch_register_t* tests, size_t tests_count)
{
    int failed_count = 0;

    for (size_t i = 0; i < tests_count; ++i)
    {
        if (strcmp(tests[i].tag, "[hidden]") == 0)
        {
            printf("SKIP: %s (%s)\n", tests[i].name, tests[i].tag);
            continue;
        }

        int index = 0, count = 0;
        do {
            ccatch_test_failed_g = false;
            count = 0;
            const char* section_name = NULL;

            tests[i].function(index++, &count, &section_name);

            const char* message = "PASSED";
            if (ccatch_test_failed_g)
            {
                ++failed_count;
                message = "FAILED";
            }

            if (section_name)
                printf("%s: %s: %s (%s)\n", message, tests[i].name, section_name, tests[i].tag);
            else
                printf("%s: %s (%s)\n", message, tests[i].name, tests[i].tag);
        } while (index < count);
    }

    return failed_count;
}

static int ccatch_compare(const void* left, const void* right)
{
    const ccatch_register_t* lh = (const ccatch_register_t*)left;
    const ccatch_register_t* rh = (const ccatch_register_t*)right;

    const int same_tag_cmp = strcmp(lh->tag, rh->tag);

    if (same_tag_cmp == 0)
        return strcmp(lh->name, rh->name);

    return same_tag_cmp;
}

int main(int argc, const char* argv[])
{
    if (ccatch_test_cases_count == 0)
    {
        printf("CCATCH: no tests found!\n");
        return 0;
    }

    if (ccatch_irrelevant_state)
    {
        printf("ERROR: not enough memory!\n");
        return -1;
    }

    qsort(ccatch_registered_test_cases, ccatch_test_cases_count, sizeof(*ccatch_registered_test_cases), ccatch_compare);

    return ccatch_run_tests(ccatch_registered_test_cases, ccatch_test_cases_count);
}

#endif // __CCATCH_WITH_MAIN_H__
