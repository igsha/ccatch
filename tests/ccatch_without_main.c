#include <ccatch.h>

#include <stdlib.h>

static bool is_visited = false;
static bool is_constructed = false;
static bool is_destructed = false;

TEST_CASE("order")
{
    is_constructed = true;

    SECTION("first")
    {
        REQUIRE(is_constructed);
        is_visited = true;
    }

    SECTION("second")
    {
        REQUIRE(is_visited);
        REQUIRE(is_destructed);
    }

    is_destructed = true;
}

typedef struct complex_object_t
{
    int* array;
    int count;
    struct complex_object_t* next;
} complex_object_t;

static int construct(complex_object_t* obj)
{
    obj->count = 10;
    obj->array = (int*)calloc(obj->count, sizeof(int));
    REQUIRE_EXIT(obj->array, -1);

    complex_object_t* chain = obj;
    for (int i = 0; i < 20; ++i)
    {
        chain->next = (complex_object_t*)calloc(1, sizeof(complex_object_t));
        chain = chain->next;
        REQUIRE_EXIT(chain, -2);
        chain->count = i + 15;
        chain->array = (int*)calloc(chain->count, sizeof(int));
        REQUIRE_EXIT(chain->array, -3);
    }

    chain->next = NULL;

    return 0;
}

static void destruct(complex_object_t* obj)
{
    complex_object_t* chain = obj->next;
    while (chain)
    {
        free(chain->array);
        complex_object_t* next = chain->next;
        free(chain);
        chain = next;
    }

    free(obj->array);
}

TEST_CASE("complex", "[tagged]")
{
    complex_object_t initial = { 0 };
    REQUIRE(construct(&initial) == 0);

    SECTION("empty")
    {
        REQUIRE("nothing to do");
    }

    destruct(&initial);
}
