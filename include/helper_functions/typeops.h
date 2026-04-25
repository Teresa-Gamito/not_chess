// operations related to its own type
#pragma once

typedef struct TypeOps
{
    void (*destroy)(void* item);
    // bool (*is_equal)(const void* item1, const void* item2);
} TypeOps;
