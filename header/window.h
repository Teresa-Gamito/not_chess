typedef struct Window {
    int width;
    int height;
    int pos_x;
    int pos_y;

} Window;

typedef enum WindowContent
{
    BOARD,
    PROPERTIES,
    HISTORY,
    TECH_TREE,
    PLAYER_1,
    PLAYER_2,
} WindowContent;

int window_get_width();
int window_get_height();

int window_get_pos_x();
int window_get_pos_y();

WindowContent window_get_content();

void window_print(Window window);
