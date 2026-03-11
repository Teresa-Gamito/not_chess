#include <stdio.h>
#include <stdlib.h>

typedef struct AppState AppState;

AppState *app_state_create();
void app_state_destroy(AppState *app_state);
