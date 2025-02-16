#include <headers.h>

void game_check_turn()
{
	
	//Exit if the turn ends and we're already in a new state
	if (gm_state!=GAME_STATE_NORMAL)
	return;
	u8 dir=input_player_check();
	if (dir!=DIR_NULL)
	{
		u8 i;
		if (dir!=DIR_NONE)
		{
			for(i = 0; i < MAX_ACTORS; i++)
			{
				Actor* a = &actors[i];
				
				if (a->x == player->x+dir_get_x(player->facing_dir) && a->y == player->y+dir_get_y(player->facing_dir))
				{
					switch (a->type)
					{
						case OBJ_BONEYM:
							player->target_x=player->x;
							player->target_y=player->y;
							gm_state=GAME_STATE_ATTACK;
							gm_timer=0;
							actor_attacking=player;
							actor_defending=a;
							return;
						case OBJ_FUSEDROPPER:
							player->target_x=player->x;
							player->target_y=player->y;
							gm_state=GAME_STATE_ATTACK;
							gm_timer=0;
							actor_attacking=player;
							actor_defending=a;
							return;
						case OBJ_POINTY: //Hack to make it so if the player walks face first into a pointy, they get what's coming to em
							if (player->facing_dir==dir_get_180(a->facing_dir))
							{
								player->target_x=player->x;
								player->target_y=player->y;
							}
							break;
					}
				}
			}
		}
		
		step_count++;
		if (step_bonus>0 && !floor_trophy_spawned)
		{
			step_bonus-=10;
		}
		update_hud=true;
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
	JOY_init();
	joypad_data=0;
	joypad_data_changed=0;
}

void input_update()
{
	joypad_data_changed=joypad_data;
	joypad_data = JOY_readJoypad(JOY_1);
}

u8 input_player_check()
{
	u8 dir = DIR_NULL;
	if (joy_held(BUTTON_RIGHT))
		dir = DIR_RIGHT;
	if (joy_held(BUTTON_UP))
		dir = DIR_UP;
	if (joy_held(BUTTON_LEFT))
		dir = DIR_LEFT;
	if (joy_held(BUTTON_DOWN))
		dir = DIR_DOWN;
	if (joy_pressed(BUTTON_BTN))
		return DIR_NONE;
	if (dir == DIR_NULL)
	{
		return DIR_NULL;
	}	
	player->facing_dir=dir;
	

	actor_face_dir(player);
	actor_target_forward(player);
	actor_set_blockmap(player,BM_LAYER_PLAYER);
	return true;
}

bool inputButtonPressed(u8 button)
{
	return ((joypad_data & button) && (joypad_data_changed & button));
}

bool inputConfirmPressed()
{
	return (inputButtonPressed(BUTTON_A) || inputButtonPressed(BUTTON_C));
}