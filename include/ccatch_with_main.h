#ifndef __CCATCH_WITH_MAIN_H__
#define __CCATCH_WITH_MAIN_H__

#include "ccatch.h"
#include "ccatch_scope.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef CCATCH_DATA_SECTION_START
#define CCATCH_DATA_SECTION_START data_start
#endif

#ifndef CCATCH_DATA_SECTION_END
#define CCATCH_DATA_SECTION_END end
#endif

uint32_t ccatch_global_object_g = 0xDEADC0DE;
bool ccatch_test_failed_g = false;

static int ccatch_compare(const void* left, const void* right)
{
    const ccatch_register_t* lh = (const ccatch_register_t*)left;
    const ccatch_register_t* rh = (const ccatch_register_t*)right;

    const int same_tag_cmp = strcmp(lh->tag, rh->tag);

    if (same_tag_cmp == 0)
        return strcmp(lh->name, rh->name);

    return same_tag_cmp;
}

static int ccatch_find_tests(ccatch_register_t** all_tests_ptr)
{
    extern void* CCATCH_DATA_SECTION_START;
    extern void* CCATCH_DATA_SECTION_END;

    size_t all_tests_count = 0;
    size_t all_tests_capacity = 10;
    ccatch_register_t* all_tests = calloc(all_tests_capacity, sizeof(ccatch_register_t));
    if (!all_tests)
        return -1;

    const uint32_t** address = (const uint32_t**)&CCATCH_DATA_SECTION_START;
    const uint32_t** end_address = (const uint32_t**)&CCATCH_DATA_SECTION_END;
    for (; address < end_address; ++address)
    {
        if (*address != &ccatch_global_object_g)
            continue;

        ccatch_register_t* st = (ccatch_register_t*)address;
        if (st->magic_number != CCATCH_MAGIC_NUMBER)
            continue;

        if (all_tests_count >= all_tests_capacity)
        {
            all_tests_capacity *= 2;
            all_tests = realloc(all_tests, all_tests_capacity * sizeof(*all_tests));
            if (!all_tests)
            {
                free(all_tests);
                return -1;
            }
        }

        all_tests[all_tests_count++] = *st;
    }

    if (all_tests_count == 0)
    {
        free(all_tests);
        all_tests = NULL;
    }

    *all_tests_ptr = all_tests;

    return all_tests_count;
}

static int ccatch_run_tests(ccatch_register_t* tests, size_t tests_count)
{
    int failed_count = 0;

    ccatch_scope_t scope = { 0 };
    ccatch_init_scope(&scope);

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

            tests[i].function(index++, &count, &section_name, &scope);
            ccatch_clear_scope(&scope);

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

    ccatch_deinit_scope(&scope);

    return failed_count;
}

int main(int argc, const char* argv[])
{
    ccatch_register_t* tests = NULL;
    int tests_count = ccatch_find_tests(&tests);

    if (tests_count == 0)
    {
        printf("CCatch: no tests found!\n");
    }
    else if (tests_count < 0)
    {
        printf("ERROR: not enough memory!\n");
        return -1;
    }
    else
    {
        qsort(tests, tests_count, sizeof(*tests), ccatch_compare);
    }

    return ccatch_run_tests(tests, (size_t)tests_count);
}

#endif // __CCATCH_WITH_MAIN_H__
