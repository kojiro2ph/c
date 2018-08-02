#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include "kdraw.h"

#define ENEMYMAX	10
#define RECT_COUNT	10
#define MOUSE_COUNT	5
#define sgn(x) ((x<0)?-1:((x>0)?1:0))


void Trig(int x,int y,int h,double prerad,int point,int number);
void Rect(int x1,int y1,int x3,int y3,int color);
void Line(int x1,int y1,int x2,int y2,int color);
void line_fast(int x1, int y1, int x2, int y2,int color);
void Circle(int x,int y,int h,int color);
void Bang(int x,int y,int h,int color);
void Check_Mouse(void);
void Status_Mouse(void);
void interrupt (*Oldvect)();
void interrupt InKey(void);


static int KeyTable[128];

extern struct TRDot_Data {
	int flag;
	int x;
	int y;
	int dx;
	int dy;
	double rad_1;
	double rad_2;
	double rad_3;
	int length;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
};


extern struct Boon_Data {
	int x;
	int y;
	int x1;
	int y1;
	int dx;
	int dy;
	int length;
	double rad;
};


extern struct ENDot_Data {
	int x;
	int y;
	int length;
	int dlength;
	int flag;

};

extern struct Mouse_Data {
	int result;
	int click;
	int x;
	int y;
}; 

extern struct TRDot_Data trdot[] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

extern struct ENDot_Data endot[] = {
	{0,0,0,0,0},
};

extern struct Boon_Data boon[] = {
	{0,0,0,0,0,0,0,0},
};

extern struct Mouse_Data mouse = {
	0,0x00,100,100
};

main() {

 int i,j,r;
 int line_color = 15;
 int rect_flag = 0;
 int rect_flag_count = 0;
 int mouse_flag_count = 0;

	VGA13h();
	Oldvect = getvect(9);
	setvect(9,InKey);
	srand((unsigned)time(NULL));

	if(Check_Mouse(),mouse.result == 0) {
		printf ("Mouse tidak ada\n");
		printf ("Program dihentikan.");
		getch();
		setvect(9,Oldvect);
		VGA03h();	
		exit(1);
 	}

	for(i=0;i<ENEMYMAX;i++) {
		trdot[i].flag = 1;
		trdot[i].x =  (rand()/32768.0*200.0)+20;
		trdot[i].y =  (rand()/32768.0*100.0)+20;
		r = rand()/32768.0*100.0;
		if(r%2 == 0) trdot[i].dx = 2;
		else trdot[i].dx = -2;
		r = rand()/32768.0*100.0;
		if(r%2 == 0) trdot[i].dy = 2;
		else trdot[i].dy = -2;
		trdot[i].length = 10;
		trdot[i].rad_1 = 90.0;
		trdot[i].rad_2 = 210.0;
		trdot[i].rad_3 = 330.0;
		trdot[i].x1 = 0;
		trdot[i].y1 = 0;
		trdot[i].x2 = 0;
		trdot[i].y2 = 0;
		trdot[i].x3 = 0;
		trdot[i].y3 = 0;
	}

	for(i=0;i<1;i++) {
		endot[i].x = 0;
		endot[i].y = 0;
		endot[i].length = 1;
		endot[i].dlength = 4;
		endot[i].flag = 0;
	}


	for(i=0;i<1;i++) {
		boon[i].x = (rand()/32768.0*200.0)+20;
		boon[i].y = (rand()/32768.0*200.0)+20;
		boon[i].x1 = 0;
		boon[i].y1 = 0;
		boon[i].dx = 0;
		boon[i].dy = 0;
		boon[i].length = 30;
		boon[i].rad = 90;
	}


	for(i=0;i<ENEMYMAX;i++) {
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_1,1,i);
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_2,2,i);
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_3,3,i);
	}

	for(i=0;i<1;i++) {
		Trig(boon[i].x,boon[i].y,boon[i].length,boon[i].rad,100,i);
	}


	while(1) {
		VsyncWait();
		if(KeyTable[0x01] == 1) break;
		if(KeyTable[0x48] == 1) continue;
		if(KeyTable[0x39] == 1) (line_color > 15) ? line_color = 0:line_color++;
		if(KeyTable[0x1d] == 1) {
			if(rect_flag_count >= 10) {
				rect_flag_count = 0;
				if(rect_flag == 0) { 
					rect_flag = 1;
				} else {
					rect_flag = 0;
					Rect(trdot[i].x1-2,trdot[i].y1-2,trdot[i].x1+2,trdot[i].y1+2,0);
					Rect(trdot[i].x2-2,trdot[i].y2-2,trdot[i].x2+2,trdot[i].y2+2,0);
					Rect(trdot[i].x3-2,trdot[i].y3-2,trdot[i].x3+2,trdot[i].y3+2,0);
				}
			} else {
				rect_flag_count++;
			}
		}

		for(i=0;i<ENEMYMAX;i++) {
			if(trdot[i].flag == 1) {				
				Rect(trdot[i].x1-2,trdot[i].y1-2,trdot[i].x1+2,trdot[i].y1+2,0);
				Rect(trdot[i].x2-2,trdot[i].y2-2,trdot[i].x2+2,trdot[i].y2+2,0);
				Rect(trdot[i].x3-2,trdot[i].y3-2,trdot[i].x3+2,trdot[i].y3+2,0);	
				line_fast(trdot[i].x1,trdot[i].y1,trdot[i].x2,trdot[i].y2,0);
				line_fast(trdot[i].x2,trdot[i].y2,trdot[i].x3,trdot[i].y3,0);
				line_fast(trdot[i].x3,trdot[i].y3,trdot[i].x1,trdot[i].y1,0);
			}
		}

		for(i=0;i<1;i++) {
			Rect(boon[i].x1-2,boon[i].y1-2,boon[i].x1+2,boon[i].y1+2,0);
		}

		Rect(mouse.x-3,mouse.y-3,mouse.x+3,mouse.y+3,0);

		for(i=0;i<1;i++) {
			if(endot[i].flag == 1) {
				Bang(endot[i].x,endot[i].y,endot[i].length,0);
			}
		}



		for(i=0;i<ENEMYMAX;i++) {
			trdot[i].x += trdot[i].dx;
			trdot[i].y += trdot[i].dy;
			if(trdot[i].rad_1 < 360)
				trdot[i].rad_1 += 3;
			else
				trdot[i].rad_1 = 0;
			if(trdot[i].rad_2 < 360)
				trdot[i].rad_2 += 3;
			else
				trdot[i].rad_2 = 0;
			if(trdot[i].rad_3 < 360)
				trdot[i].rad_3 += 3;
			else
				trdot[i].rad_3 = 0;
		}

		for(i=0;i<1;i++) {
			boon[i].x += boon[i].dx;
			boon[i].y += boon[i].dy;
			if(boon[i].rad < 360)
				boon[i].rad += 3;
			else
				boon[i].rad = 0;
			
		}

		for(i=0;i<ENEMYMAX;i++) {
			if(trdot[i].x1 <= 3) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y1 <= 3) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
			if(trdot[i].x1 >= 317) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y1 >= 197) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
			if(trdot[i].x2 <= 3) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y2 <= 3) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
			if(trdot[i].x2 >= 317) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y2 >= 197) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
			if(trdot[i].x3 <= 3) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y3 <= 3) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
			if(trdot[i].x3 >= 317) {trdot[i].dx = -trdot[i].dx;trdot[i].x += trdot[i].dx*4;break;}
			if(trdot[i].y3 >= 197) {trdot[i].dy = -trdot[i].dy;trdot[i].y += trdot[i].dy*4;break;}
		}

		for(i=0;i<ENEMYMAX;i++) {
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_1,1,i);
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_2,2,i);
		Trig(trdot[i].x,trdot[i].y,trdot[i].length,trdot[i].rad_3,3,i);
		}

		for(i=0;i<1;i++) {
			Trig(boon[i].x,boon[i].y,boon[i].length,boon[i].rad,100,i);
		}


		Status_Mouse();	
		
		if(mouse.click == 0x01) {
			if(mouse_flag_count >= MOUSE_COUNT) {
				mouse_flag_count = 0;
				for(i=0;i<1;i++) {
					if(endot[i].flag == 0) {
						endot[i].flag = 1;
						endot[i].x = mouse.x;
						endot[i].y = mouse.y;
					}
				}
			} else {
				mouse_flag_count++;
			}
	
	
			for(i=0;i<ENEMYMAX;i++) {
				if(trdot[i].flag == 1) {
					if(mouse.x >= trdot[i].x - trdot[i].length && mouse.x <= trdot[i].x + trdot[i].length &&
					   mouse.y >= trdot[i].y - trdot[i].length && mouse.y <= trdot[i].y + trdot[i].length) {
						trdot[i].flag = 0;
					}
				}
			}
		}

		for(i=0;i<ENEMYMAX;i++) {
			if(trdot[i].flag == 1) {
				if(rect_flag == 1) {
					Rect(trdot[i].x1-2,trdot[i].y1-2,trdot[i].x1+2,trdot[i].y1+2,14);
					Rect(trdot[i].x2-2,trdot[i].y2-2,trdot[i].x2+2,trdot[i].y2+2,14);
					Rect(trdot[i].x3-2,trdot[i].y3-2,trdot[i].x3+2,trdot[i].y3+2,14);
				}
				line_fast(trdot[i].x1,trdot[i].y1,trdot[i].x2,trdot[i].y2,line_color);
				line_fast(trdot[i].x2,trdot[i].y2,trdot[i].x3,trdot[i].y3,line_color);
				line_fast(trdot[i].x3,trdot[i].y3,trdot[i].x1,trdot[i].y1,line_color);
			}
		}

		for(i=0;i<1;i++) {
			Rect(boon[i].x1-2,boon[i].y1-2,boon[i].x1+2,boon[i].y1+2,line_color);
		}


		Rect(mouse.x-3,mouse.y-3,mouse.x+3,mouse.y+3,14);


		for(i=0;i<1;i++) {
			if(endot[i].flag == 1) {
				endot[i].length += endot[i].dlength;
				Bang(endot[i].x,endot[i].y,endot[i].length,10);
				if(endot[i].length > 17) {
				Bang(endot[i].x,endot[i].y,endot[i].length,0);
					endot[i].flag = 0;;
					endot[i].x = 0;
					endot[i].y = 0;
					endot[i].length = 0;
				}
			}
		}
/*
		gotoxy(1,1);
		for(i=0;i<ENEMYMAX;i++) {
			if(trdot[i].x > 317 || trdot[i].x < 3 || trdot[i].y > 197 || trdot[i].x < 3) {
				printf("%2d %3d %3d\n",i,trdot[i].x,trdot[i].y);
			}
		}
*/	
		
	}

	setvect(9,Oldvect);
	VGA03h();

}





void Line(int x1, int y1, int x2, int y2, int color)
{
  int dx,dy,sdx,sdy,px,py,dxabs,dyabs,i;
  float slope;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    slope=(float)dy / (float)dx;
    for(i=0;i!=dx;i+=sdx)
    {
      px=i+x1;
      py=slope*i+y1;
      if(px > 0 && px < 320 && py > 0 && py < 200)
      	Pset(px,py,color);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    slope=(float)dx / (float)dy;
    for(i=0;i!=dy;i+=sdy)
    {
      px=slope*i+x1;
      py=i+y1;
      if(px > 0 && px < 320 && py > 0 && py < 200)
      	Pset(px,py,color);
    }
  }
}


void line_fast(int x1, int y1, int x2, int y2,int color)
{
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;



  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      Pset(px,py,color);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      Pset(px,py,color);
    }
  }
}

void Rect(int x1,int y1,int x3,int y3,int color) {

	line_fast(x1,y1,x3,y1,color);
	line_fast(x1,y1,x1,y3,color);
	line_fast(x1,y3,x3,y3,color);
	line_fast(x3,y1,x3,y3,color);
}


void Trig(int x,int y,int h,double prerad,int point,int number) {

  double pi;
  double rad;

	pi = 4*atan(1);

	switch(point) {
		case 1:
			rad = prerad*(pi/180);
			trdot[number].x1 = ceil(h*sin(rad)+x);
			trdot[number].y1 = ceil(h*cos(rad)+y);
			break;
		case 2:
			rad = prerad*(pi/180);
			trdot[number].x2 = ceil(h*sin(rad)+x);
			trdot[number].y2 = ceil(h*cos(rad)+y);
			break;
		case 3:
			rad = prerad*(pi/180);
			trdot[number].x3 = ceil(h*sin(rad)+x);
			trdot[number].y3 = ceil(h*cos(rad)+y);
			break;
		case 100:
			rad = prerad*(pi/180);
			boon[number].x1 = ceil(h*sin(rad)+x);
			boon[number].y1 = ceil(h*cos(rad)+y);
			break;

		}

}

void Circle(int x,int y,int h,int color) {

  double i;
  double pi;
  double rad;

	pi = 4*atan(1);

	for(i=0;i<360;i++) {
		rad = i*(pi/180);
		Pset(h*sin(rad)+x,h*cos(rad)+y,color);
	}


}


void Bang(int x,int y,int h,int color) {

  double i;
  double pi;
  double rad;

	pi = 4*atan(1);

	for(i=0;i<360;i+=10) {
		rad = i*(pi/180);
		/*
		Pset(h*sin(rad)+x,h*cos(rad)+y,color);
		*/
		line_fast(mouse.x,mouse.y,h*sin(rad)+x,h*cos(rad)+y,color);
	}


}

void interrupt InKey(void) {
	int key;
	bdos(0x0c,0,0);
	key = inp(0x60);
	if(key != 0xe0) {
		if((key & 0x80) == 0) {
			KeyTable[key & 0x7f] = 1;
		} else {
			KeyTable[key & 0x7f] = 0;
		}
	}
	outp(0x20,0x20);
}



		/* mouse ruotuine */


void Check_Mouse(void)
{
 union REGS inregs, outregs;
 inregs.x.ax=0x00;
 int86 (0x33, &inregs, &outregs);
 mouse.result=outregs.x.bx;

} 



void Status_Mouse(void)
{
 union REGS inregs, outregs;
  inregs.x.ax=0x03;
 int86 (0x33, &inregs, &outregs);
 mouse.click = outregs.x.bx;
 mouse.x = (outregs.x.cx)/2;
 mouse.y = outregs.x.dx;
}