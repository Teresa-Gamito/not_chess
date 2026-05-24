#include "include/appstate.h"
#include "helper_functions/global_variables.h"

struct AppState
{
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    InputState* input;

    Game* game;
    GameUI* game_ui;
};

AppState* app_create()
{
    AppState* app = (AppState*)SDL_malloc(sizeof(AppState));
    verify(app == NULL, "AppState could not be created: malloc");

    TTF_Init();

    app->sdl_window = SDL_CreateWindow(
        APP_NAME,
        APP_WINDOW_WIDTH,
        APP_WINDOW_HEIGHT,
        APP_WINDOW_FLAGS
    );
    verify(app->sdl_window == NULL, "SDL_Window could not be created");

    app->sdl_renderer = SDL_CreateRenderer(app->sdl_window, NULL);
    verify(app->sdl_renderer == NULL, "SDL_Renderer could not be created");

    SDL_SetDefaultTextureScaleMode(app->sdl_renderer, SDL_SCALEMODE_PIXELART);

    app->game = NULL;
    app->game_ui = NULL;

    app->input = input_create();

    return app;
}

void app_init(AppState* app)
{
    verify_app_state(app);

    app->game = game_create();
    game_start(app->game);

    app->game_ui = game_ui_create(app->game, app->sdl_renderer);

}

void app_destroy(AppState* app)
{
    verify_app_state(app);

    SDL_DestroyWindow(app->sdl_window);
    SDL_DestroyRenderer(app->sdl_renderer);

    input_destroy(app->input);

    game_ui_destroy(app->game_ui);
    game_destroy(app->game);

    TTF_Quit();

    SDL_free(app);
}


static void update_global_variables(AppState* app)
{
    int app_width;
    int app_height;
    SDL_GetWindowSize(app->sdl_window, &app_width, &app_height);
    g_app_window_width = app_width;
    g_app_window_height = app_height;
    if ((float) APP_WINDOW_WIDTH / APP_WINDOW_HEIGHT < g_app_window_width / g_app_window_height)
    {
        g_app_scale = g_app_window_width / APP_WINDOW_WIDTH;
    }
    else
    {
        g_app_scale = g_app_window_height / APP_WINDOW_HEIGHT;
    }
    g_menu_font_size = FONT_SIZE_MENU * g_app_scale;
    g_game_font_size = FONT_SIZE * g_app_scale;
}

static void update_cheats(AppState* app)
{
    InputState* input = app->input;

    if (keyboard_get_pressed(input, SDL_SCANCODE_1))
    {
        infinite_points = !infinite_points;
    }
    if (keyboard_get_pressed(input, SDL_SCANCODE_2))
    {
        can_purchace_multiple_times = !can_purchace_multiple_times;
    }
}

void app_update(AppState* app)
{
    verify_app_state(app);

    InputState* input = app->input;

    switch (game_ui_update(input, app->game_ui))
    {
        case GAME_RESULT_CONTINUE:
            break;

        case GAME_RESULT_WIN_WHITE:
            SDL_Log("WHITE WINS!");
            app_quit(NULL, NULL);
            break;

        case GAME_RESULT_WIN_BLACK:
            SDL_Log("BLACK WINS!");
            app_quit(NULL, NULL);
            break;

        case GAME_RESULT_EXIT:
            app_quit(NULL, NULL);
            break;
    }

    update_global_variables(app);
    update_cheats(app);
}

void app_render(AppState* app)
{
    verify_app_state(app);

    SDL_Renderer* renderer = app->sdl_renderer;

    SDL_SetRenderDrawColor(renderer, 12, 23, 34, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    game_ui_render(renderer, app->game_ui);

    SDL_RenderPresent(renderer);
}

InputState* app_get_inputstate(const AppState* app)
{
    verify_app_state(app);

    return app->input;
}

void verify_app_state(const AppState* app)
{
    verify(app == NULL, "AppState does not exist");
}
