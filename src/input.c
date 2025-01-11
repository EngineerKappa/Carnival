#include <headers.h>

void game_check_turn()
{
	
	//Exit if the turn ends and we're already in a new state
	if (gm_state!=GAME_STATE_NORMAL)
	return;
	
	if (input_player_check())
	{
		step_count++;
		gm_state=GAME_STATE_MOVE;
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
		SPR_setFrame(player->sprite,player->frame);
	}

	if (joypad_data==0)
	{
		turn_updated=false;
	}
}

void input_init()
{
	joypad_data=0;
}

void input_update(u16 joy, u16 changed, u16 state)
{
	joypad_data = state;
}

bool input_player_check()
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
	player->facing_dir=dir;
	actor_face_dir(player);
	actor_turn(player);
	return true;
}

