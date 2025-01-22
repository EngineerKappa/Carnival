#ifndef _INPUT_H_
#define _INPUT_H_



u16 joypad_data;
u16 joypad_data_changed;
bool mouse_enabled;
u16 cursor_x;
u16 cursor_y;

Sprite* s_mouse_cursor;

void input_update();
bool inputConfirm();
bool inputButtonPressed(u8 button);
bool inputConfirmPressed();
bool inputBack();
bool inputBackPressed();
void input_init();
u8 input_player_check();
void game_check_turn();


#define joy_pressed(b) ((joypad_data ^ joypad_data_changed) & b) && ((joypad_data) & b)
#define joy_held(b) (joypad_data & b)


#endif // _INPUT_H_
