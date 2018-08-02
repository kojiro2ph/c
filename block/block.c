#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <time.h>

#include "init.h"
#include "control.c"





/***************************************************************
		       Main Function
***************************************************************/

void main(void) 
{

	VGA13h();	/* Set Video Mode */
	Oldvect = getvect(9);
	setvect(9,InKey);
	Stage();
	setvect(9,Oldvect);
	VGA03h();	/* Return to Text Mode */

}





