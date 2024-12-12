#include <genesis.h>
#include <input.h>
#include <gfx.h>

inline void input_update()
{
	joypad_data = JOY_readJoypad(JOY_1);
}

void input_update_pressed()
{
	joypad_data_previous = joypad_data;
	input_update();
}

bool inputConfirm()
{
		return (joypad_data & BUTTON_A) || (joypad_data & BUTTON_C);
}

bool inputConfirmPressed()
{
	bool apress = ((joypad_data ^ joypad_data_previous) & BUTTON_A) && ((joypad_data) & BUTTON_A);
	bool cpress = ((joypad_data ^ joypad_data_previous) & BUTTON_C) && ((joypad_data) & BUTTON_C);

	switch (mouse_enabled)
	{
		case true:
		return apress;
	default:
		return apress || cpress;
	}
	
}

bool inputBack()
{
		return (joypad_data & BUTTON_B);
}

bool inputBackPressed()
{
		return ((joypad_data ^ joypad_data_previous) & BUTTON_B) && ((joypad_data) & BUTTON_B);
}

void input_init()
{
}