#include <genesis.h>
#include <input.h>
#include <gfx.h>
#include <actors.h>

void run_turn()
{
	if (player_check_turn() && ((turn_updated==false) | (delayed_auto_shift >= AUTO_SHIFT_LIMIT) ))
	{
		if (delayed_auto_shift >= AUTO_SHIFT_LIMIT)
			delayed_auto_shift = AUTO_SHIFT_RESTART;
		
		actors_update();
		turn_updated=true;

		if (player->frame == 0)
		{
			player->frame = 1;
		}
		else
		{
			player->frame = 0;
		}

	}

	if (joypad_data==0)
	{
		turn_updated=false;
		delayed_auto_shift=0;
	}
}


void input_init()
{
	delayed_auto_shift=0;
	joypad_data=0;
}

void input_update(u16 joy, u16 changed, u16 state)
{
	joypad_data = state;
}

bool player_check_turn()
{
	u8 dir = NULL;

	if (joypad_data & BUTTON_RIGHT)
		dir = DIR_RIGHT;
	if (joypad_data & BUTTON_UP)
		dir = DIR_UP;
	if (joypad_data & BUTTON_LEFT)
		dir = DIR_LEFT;
	if (joypad_data & BUTTON_DOWN)
		dir = DIR_DOWN;

	if (dir == NULL)
	{
		return false;
	}	
	delayed_auto_shift++;
	
	player->facing_dir=dir;
	player->target_x=player->x;
	player->target_y=player->y;
	
	

	switch (dir) 
	{
		case DIR_RIGHT:
			player->target_x=player->x+1;
			SPR_setAnimAndFrame(player->sprite, 1, player->frame);
			SPR_setHFlip(player->sprite,false);
			break;
		case DIR_UP:
			player->target_y=player->y-1;
			SPR_setAnimAndFrame(player->sprite, 2, player->frame);
			SPR_setHFlip(player->sprite,false);
			break;
		case DIR_DOWN:
			player->target_y=player->y+1;
			SPR_setAnimAndFrame(player->sprite, 0, player->frame);
			SPR_setHFlip(player->sprite,false);
			break;
		case DIR_LEFT:
			player->target_x=player->x-1;
			SPR_setAnimAndFrame(player->sprite, 1, player->frame);
			SPR_setHFlip(player->sprite,true);
			break;
	}
	

	return true;
}

