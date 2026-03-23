#pragma once

typedef struct MenuState MenuState;

MenuState* menustate_create();
void menustate_destroy(MenuState* menustate);
