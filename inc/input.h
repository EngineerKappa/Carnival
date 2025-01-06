#ifndef _INPUT_H_
#define _INPUT_H_



u16 joypad_data;
u16 joypad_data_previous;
bool mouse_enabled;
u16 cursor_x;
u16 cursor_y;
u8 delayed_auto_shift;
#define AUTO_SHIFT_RESTART 5
#define AUTO_SHIFT_LIMIT 10

Sprite* s_mouse_cursor;

void input_update(u16 joy, u16 changed, u16 state);
bool inputConfirm();
bool inputConfirmPressed();
bool inputBack();
bool inputBackPressed();
void input_init();
bool input_player_check();
void run_turn();



#endif // _INPUT_H_
