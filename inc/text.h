enum {
    TEXT_STATE_INACTIVE,
    TEXT_STATE_TYPING
};

#define TEXTBOX_X 2
#define TEXTBOX_Y 22
#define TEXTBOX_WIDTH 26
#define TEXTBOX_HEIGHT 5

u16 text_copy_count;
u8 textbox_state;
u8 text_length;
char* p_text_current;
char char_current;
u8 text_x;
u8 text_y;
u8 textbox_anim;

void text_check_room();
void textbox_draw_rect();
void text_init();
void text_update();
void textbox_draw_char(char destination_char);