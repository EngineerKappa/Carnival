#include <headers.h>

int main()
{
	system_init();
	
	while(1)
	{     
		system_update();   
		SPR_update();
		SYS_doVBlankProcess();
		transition_animate();
		if (gm_current==GM_GAME)
		{
			game_draw_hud_text();
		}
		input_update();
	}
	return (0);
}
