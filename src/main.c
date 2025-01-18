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
		input_update();
	}
	return (0);
}
