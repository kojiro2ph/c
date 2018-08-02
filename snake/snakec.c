/*************************************************************
			Sub Routine
*************************************************************/


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


void Wait(int t) {
	int i=0;
	
	while(1) {
		VsyncWait();
		i++;
		if((i/70) == t) break;
	}
}


void GameOver() {
 int i,j;

	Toufu(270,5,30,10,0);
	Num(270,5,0);
	Rect(19,19,301,181,14);

	Toufu(235,5,30,10,0);
	Num(235,5,0);
	Rect(233,3,265,16,2);

	gotoxy(1,1);
	printf("                         ");
	gotoxy(1,1);
	printf("BOOM!!!");

	for(i=0;i<32;i++) {
		for(j=0;j<56;j++) {
			Rect(j*5+20,i*5+20,j*5+20+snake.w,i*5+20+snake.h,snake.col);
			SoundOut(1000,50);
		}
	}
	Wait(2);


	Toufu(WALLL+1,WALLT+1,WALLW,WALLH,0);

	gotoxy(1,1);
	printf("        ");

	score = 0;
	snake.gameover_flag = 1;
	random.flag = 1;
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


void StandBy() {
 int i;

	snake.x 		= 100;
	snake.y 		= 100;
	snake.w 		= 5;
	snake.h 		= 5;
	snake.col		= 15;
	snake.spd 		= 5;
	snake.spd_count 	= 0;
	snake.dst 		= RIGHT;
	snake.length 		= 1;
	snake.gameover_flag 	= 0;
 
	for(i=0;i<100;i++) {
		tail[i].x 	= 0;
		tail[i].y 	= 0;
		tail[i].w 	= 5;
		tail[i].h 	= 5;
		tail[i].col 	= 15;
	}


	for(i=snake.length-1;i>=0;i--) {
		if(i==0) {
			tail[i].x = snake.x;
			tail[i].y = snake.y;
			tail[i].w = snake.w;
			tail[i].h = snake.h;
			tail[i].col = snake.col;
		} else {
			tail[i].x = tail[i-1].x;
			tail[i].y = tail[i-1].y;
			tail[i].w = tail[i-1].w;
			tail[i].h = tail[i-1].h;
			tail[i].col = tail[i-1].col;
		}

	}

	Rect(19,19,301,181,14);
	Rect(268,3,301,16,2);
	Rect(233,3,265,16,2);
	Toufu(270,5,30,10,0);
	Toufu(235,5,30,10,0);
	Num(270,5,score);
	Num(235,5,snake.length);
	for(i=0;i<=snake.length-1;i++) {
		if(snake_style == 1) {
			Rect(tail[i].x,tail[i].y,tail[i].x+tail[i].w,tail[i].y+tail[i].h,snake.col);
		} else {
			Toufu(tail[i].x,tail[i].y,tail[i].w,tail[i].h,snake.col);
		}
	}





	while(KeyTable[0x39] == 0)
			;

}

void Num(int x,int y,int num) {
   int keta;
	keta = num/10000;
	DDraw(x,y,number[keta]);

	num -= keta*10000;
	keta = num/1000;
	DDraw(x+6,y,number[keta]);

	num -= keta*1000;
	keta = num/100;
	DDraw(x+12,y,number[keta]);
	
	num -= keta*100;
	keta = num/10;
	DDraw(x+18,y,number[keta]);

	num -= keta*10;
	keta = num/1;
	DDraw(x+24,y,number[keta]);
}


void SoundOut(int Hz,int OutTime) {
 int dt;

	dt = 1193280L/Hz;        	/* 3000Hzの音         */
	outp(0x43, 0xb6);           	/* タイマー設定の準備 */
	outp(0x42, dt%256);         	/* 下位バイト出力     */
	outp(0x42, dt/256);       	/* 上位バイト出力     */
	outp(0x61,inp(0x61)|0x03); 	/* スピーカーオン     */

	 if(OutTime == 0) {
		VsyncWait();
	 } else {
		delay(OutTime);	
	}

	outp(0x61,inp(0x61)&0xFC);  	/* スピーカーオフ     */
}
/*******************************************************************
			Start Demo
*******************************************************************/

void StartDemo(void) {

	int i=0,j=0;
	char moji[100] = "WELCOME TO SNAKE WORLD!!!";

	printf("\033[45m");
	while(moji[i]) {
		putchar(moji[i]);
		SoundOut(3000,0);
		for(j=0;j<=2;j++) 
			VsyncWait();
		i++;
	}
	printf("\033[m");

	Wait(1);
	gotoxy(1,1);
	printf("                         ");
	Wait(1);
	
}

/*******************************************************************
			Stage Control
*******************************************************************/

void Stage(void) {
 int i;

StartDemo();

/* -------------------- */
_STANDBY_:
	StandBy();
/* -------------------- */


	while(1) {
	/* 1/70 sec wait... */

		VsyncWait();
	/* Create Isolated Tail */

		if(random.flag == 1) {
			random.x = 0;
			random.y = 0;
			while(random.x < 20 || random.x > 295 || random.y < 20 || random.y > 175) {
				random.x = ceil(rand()/32768.0*59.0)*5;
				random.y = ceil(rand()/32768.0*35.0)*5;
			}
			if(snake_style == 1) {
				Rect(random.x,random.y,random.x+random.w,random.y+random.h,random.col);
			} else {
				Toufu(random.x,random.y,random.w,random.h,random.col);
			}
			random.flag = 0;
		}
	/* Key Check */

		if(KeyTable[0x01] == 1) break;
		if(KeyTable[0x48] == 1 && (snake.dst != DOWN || snake.length == 1)) snake.dst = UP; /* UP */
		if(KeyTable[0x50] == 1 && (snake.dst != UP || snake.length == 1)) snake.dst = DOWN; /* DOWN */
		if(KeyTable[0x4d] == 1 && (snake.dst != LEFT || snake.length == 1)) snake.dst = RIGHT; /* RIGHT */
		if(KeyTable[0x4b] == 1 && (snake.dst != RIGHT || snake.length == 1)) snake.dst = LEFT; /* LEFT */
		if(KeyTable[0x1d] == 1) (snake_style == 1) ? (snake_style = 0):(snake_style = 1),Toufu(WALLL+1,WALLT+1,WALLW,WALLH,0),delay(100); 
	/* Speed control */

		if(++snake.spd_count >= snake.spd) snake.spd_count = 0;else continue;
		if(KeyTable[0x0c] == 1) snake.spd--; 
		if(KeyTable[0x0d] == 1) snake.spd++;
		
		if(snake.length == 10) snake.spd=4;
		if(snake.length == 20) snake.spd=3;
		if(snake.length == 30) snake.spd=2;
		if(snake.length == 40) snake.spd=1;

		
			
	/* Delete Picture */

		for(i=0;i<=snake.length-1;i++) {
			if(snake_style == 1) {
				Rect(tail[i].x,tail[i].y,tail[i].x+tail[i].w,tail[i].y+tail[i].h,0);
				Toufu(tail[i].x,tail[i].y,tail[i].w,tail[i].h,0);
			} else {
				Toufu(tail[i].x,tail[i].y,tail[i].w,tail[i].h,0);
			}
		}
	/* Move snake */

		switch (snake.dst) {
			case UP:	snake.y -= 5;break;
			case DOWN:	snake.y += 5;break;
			case RIGHT:	snake.x += 5;break;
			case LEFT:	snake.x -= 5;break;
		}
	/* Position Check */

		if(snake.x < 0) snake.x = 0;
		if(snake.x + snake.w > 320) snake.x = 320 - snake.w;
		if(snake.y < 0) snake.y = 0;
		if(snake.y + snake.h > 200) snake.y = 200 - snake.h;

		if(snake.x == random.x && snake.y == random.y) {
			snake.x = random.x;
			snake.y = random.y;
			snake.length++;
			random.flag = 1;
			if(snake.length <= 10)  score+=6;
			if(snake.length > 10 && snake.length <= 20) score+=13;
			if(snake.length > 20 && snake.length <= 30) score+=42;
			if(snake.length > 30 && snake.length <= 40) score+=94;
			if(snake.length > 40 && snake.length <= 50) score+=126;
			if(snake.length > 50) score+=258;

			score += rand()/32768.0*10;

			Toufu(270,5,30,10,0);
			Num(270,5,score);
			Toufu(235,5,30,10,0);
			Num(235,5,snake.length);
			sound_flag = 1;
		}


		if(snake.x + snake.w >= WALLR || snake.x <= WALLL || snake.y + snake.h >= WALLB || snake.y <= WALLT) GameOver();

		for(i=0;i<=snake.length-1;i++) {
			if(snake.x == tail[i].x && snake.y == tail[i].y) {
					GameOver();
					break;
				
			}
		}

		if(snake.gameover_flag == 1) goto _STANDBY_;
					
	/* Change Position Data */

		for(i=snake.length-1;i>=0;i--) {
			if(i==0) {
				tail[i].x = snake.x;
				tail[i].y = snake.y;
				tail[i].w = snake.w;
				tail[i].h = snake.h;
				tail[i].col = snake.col;
			} else {
				tail[i].x = tail[i-1].x;
				tail[i].y = tail[i-1].y;
				tail[i].w = tail[i-1].w;
				tail[i].h = tail[i-1].h;
				tail[i].col = tail[i-1].col;
			}

		}

	/* Check Status */
		if(snake.length == 5) snake.col	=14;
		if(snake.length == 10) snake.col=13;
		if(snake.length == 15) snake.col=12;
		if(snake.length == 20) snake.col=11;
		if(snake.length == 25) snake.col=10;
		if(snake.length == 30) snake.col=9;
		if(snake.length == 35) snake.col=8;
		if(snake.length == 40) snake.col=7;
		if(snake.length == 45) snake.col=6;
		if(snake.length == 50) snake.col=5;
		random.col = snake.col;
	

	/* Draw Picture */ 
		Rect(19,19,301,181,14);
		if(snake_style == 1) {
			Rect(random.x,random.y,random.x+random.w,random.y+random.h,random.col);

		} else {
			Toufu(random.x,random.y,random.w,random.h,random.col);
		}
		for(i=0;i<=snake.length-1;i++) {
			if(snake_style == 1) {
				Rect(tail[i].x,tail[i].y,tail[i].x+tail[i].w,tail[i].y+tail[i].h,snake.col);
			} else {
				Toufu(tail[i].x,tail[i].y,tail[i].w,tail[i].h,snake.col);
			}
		}
	/* Effect Control */
		if(sound_flag > 0) {
			SoundOut(2500,0);
			sound_flag = 0;
		}
	}
}