#include "includes.h"


int main(void)
{
	int flag=1;
	init_all_and_POST();
	
	
	/* Loop forever */
	for (;;)
	{
	//control_steer_helm_1();

#if 1
		/* ÷¥––‘∂≥Ã√¸¡Ó */
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+5);
		}
#endif
	}
}