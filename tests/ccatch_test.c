#include <ccatch_with_main.h>

#include <stdlib.h>

TEST_CASE("skip test case", "[hidden]")
{
    REQUIRE(7 == 9);
}

TEST_CASE("sections")
{
    SECTION("the first section")
    {
        int i = -2;
        REQUIRE(i == -2);
    }

    SECTION("the second section")
    {
        int i = 4458;
        REQUIRE(i == 4458);
    }

    SECTION("the third section")
    {
        int i = 3214;
        REQUIRE(i == 3214);
    }
}

static bool is_freed = false;

static void free_mem(void* mem)
{
    free(mem);
    is_freed = true;
}

TEST_CASE("default scenario")
{
    const size_t count = 437;
    char* m = malloc(count);
    char* j = malloc(16);
    SCOPE_ADD(m, free, j, free_mem);

    SECTION("not NULL")
    {
        REQUIRE(m != NULL);
    }

    SECTION("try memset")
    {
        memset(m, 0, count);
        REQUIRE(m[400] == 0);
    }

    SECTION("at least once free")
    {
        REQUIRE(is_freed);
    }
}
