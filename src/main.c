#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <system.h>
#include <gm_game.h>
#include <background.h>
#include <input.h>
#include <actors.h>
#include <transition.h>


int main()
{
	system_init();
	while(1)
	{     
		transition_animate();
		system_update();   
		SPR_update();
		
		SYS_doVBlankProcess();
		
	}
	return (0);
}
