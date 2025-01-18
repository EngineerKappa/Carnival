enum {
    MENU_STATE_INACTIVE,
    MENU_STATE_ACTIVE,
    MENU_STATE_PRESSED,
};
struct MenuOption;

#define MENU_TEXT_SIZE 10

typedef void (*MenuAction)();

typedef struct MenuOption {
    MenuAction menu_action;
    char label[MENU_TEXT_SIZE];
} MenuOption;


MenuOption menu_list[10];
u8 menu_state;
u8 menu_option;
u8 menu_option_previous;
u8 menu_x;
u8 menu_y;
u8 menu_size;
u8 menu_timer;
void menu_init();
void menu_update();
void menu_create(u8 x, u8 y);
void menu_add_option(const char *label, FunctionPointer menu_action);
void menu_draw(bool flash);
void menu_clear();
