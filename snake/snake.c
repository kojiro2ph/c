#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <string.h>



#define RIGHT	2 
#define LEFT	4 
#define UP 	1
#define DOWN	3
#define sgn(x) ((x<0)?-1:((x>0)?1:0))

#include "snakei.c"
#include "kdraw.h"
#include "snakec.c"




void main(void) {



	VGA13h();
	Oldvect = getvect(9);
	setvect(9,InKey);
	srand((unsigned)time(NULL));
	Stage();
	setvect(9,Oldvect);
	VGA03h();


}


