/* Copyright 2021 OpenFive */
/*
 *  XM6 based CDNN application
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"

//extern unsigned char PrintEnable;

int main(int argc, char *argv[])
{
	int status;

	//PrintEnable = 1;
	CEVAPRINTF("Start CDNN Application\n");
	CEVAPRINTF("%s\n", __TIMESTAMP__);
//	PrintEnable = 0;

	status = AppProcess();

	if (status != 0)
	{
		CEVAPRINTF("Failed : %d \n", status);
	}
	else
	{
		CEVAPRINTF("Finished\n");
	}

	__asm__ __volatile__("verifend\n");

	while(1)
	{
	__asm__ __volatile__("nop");
	}

	return 0;
}
