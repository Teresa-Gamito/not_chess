#include "include/game/log/log.h"

struct GameLog
{
    Vector* logs;
};

GameLog* gamelog_create()
{
    GameLog* log = SDL_malloc(sizeof(GameLog));
    verify(log == NULL, "GameLog could not be created: malloc");

    log->logs = vector_create(default_ops());

    return log;
}
void gamelog_destroy(GameLog* log)
{
    verify_gamelog(log);

    vector_destroy(log->logs);
    SDL_free(log);
}

void gamelog_add(GameLog* log, const char* msg, ...)
{
    verify_gamelog(log);

    va_list args;
    va_start(args, msg);
    char* log_msg;
    SDL_asprintf(&log_msg, msg, args);
    vector_add(log->logs, SDL_strdup(log_msg));
    va_end(args);
    SDL_free(log_msg);
}
const char* gamelog_get(const GameLog *log)
{
    verify_gamelog(log);
    if (vector_get_count(log->logs) <= 0)
    {
        return NULL;
    }

    char* full_log = "";
    for (int i = vector_get_count(log->logs) - 1; i >= 0; i--)
    {
        char* curr_msg = vector_get_at(log->logs, i);
        SDL_asprintf(&full_log, "%s%s\n", full_log, curr_msg);
    }
    return full_log;
}

void verify_gamelog(const GameLog* log)
{
    verify(log == NULL, "GameLog does not exist");
}
