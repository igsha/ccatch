#ifndef __CCATCH_SCOPE_H__
#define __CCATCH_SCOPE_H__

#ifndef __CCATCH_WITH_MAIN_H__
#error "Do not include this file directly. Use ccatch_with_main.h instead"
#endif

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct ccatch_scope_object_t
{
    void* object;
    ccatch_scope_destructor_f* destructor;
} ccatch_scope_object_t;

struct ccatch_scope_t
{
    ccatch_scope_object_t* objects;
    size_t count;
    size_t capacity;
};

static void ccatch_init_scope(ccatch_scope_t* scope)
{
    scope->capacity = 20;
    scope->objects = (ccatch_scope_object_t*)calloc(scope->capacity, sizeof(ccatch_scope_object_t));
    if (!scope->objects)
        assert(!"Internal memory error at initialization!");

    scope->count = 0;
}

void ccatch_add_object_to_scope(ccatch_scope_t* scope, void* object, ccatch_scope_destructor_f* destructor)
{
    if (scope->count >= scope->capacity)
    {
        void* new_objects = realloc(scope->objects, 2 * scope->capacity * sizeof(ccatch_scope_object_t));
        if (!new_objects)
            assert(!"Internal memory error!");

        scope->objects = (ccatch_scope_object_t*)new_objects;
        scope->capacity *= 2;
    }

    ccatch_scope_object_t* obj = &scope->objects[scope->count++];
    obj->object = object;
    obj->destructor = destructor;
}

static void ccatch_clear_scope(ccatch_scope_t* scope)
{
    for (size_t i = 0; i < scope->count; ++i)
    {
        ccatch_scope_object_t* object = &scope->objects[i];
        object->destructor(object->object);
        object->object = NULL;
        object->destructor = NULL;
    }

    scope->count = 0;
}

static void ccatch_deinit_scope(ccatch_scope_t* scope)
{
    ccatch_clear_scope(scope);
    free(scope->objects);
    scope->capacity = 0;
}

#endif // __CCATCH_SCOPE_H__
