#include <genesis.h>
#include <input.h>
#include <gfx.h>

void input_update(u16 joy, u16 changed, u16 state)
{
	joypad_data = JOY_readJoypad(JOY_1);
}
