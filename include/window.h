typedef enum WindowContent {
    BOARD,
    TREE,
    PROPERTIES,
    RULES,
    HISTORY,
    INFO_TAB,
    PLAYER_1,
    PLAYER_2,
} WindowContent;

typedef struct Window Window;

// create / destroy
Window* window_create();
void window_destroy(Window* window);

// set
void window_set_size(Window* window, double width, double height);
void window_set_posistion(Window* window, double pos_x, double pos_y);

// get
int window_get_width(Window* window);
int window_get_height(Window* window);

int window_get_pos_x(Window* window);
int window_get_pos_y(Window* window);

WindowContent window_get_content(Window* window);

void window_print(Window window);
