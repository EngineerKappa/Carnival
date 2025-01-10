#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <system.h>
#include <gm_game.h>
#include <background.h>
#include <input.h>
#include <actors.h>
#include <transition.h>
#include <room.h>


int main()
{
	system_init();
	
	while(1)
	{     
		
		system_update();   
		SPR_update();
		
		SYS_doVBlankProcess();
		transition_animate();
		
	}
	return (0);
}
