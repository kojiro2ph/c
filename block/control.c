void VGA13h(void)
{
	union REGS inregs,outregs;
	inregs.h.ah = 0x00;
	inregs.h.al = 0x13;
	int86(0x10,&inregs,&outregs);
}


void VGA03h(void)
{
	union REGS inregs,outregs;
	inregs.h.ah = 0x00;
	inregs.h.al = 0x03;
	int86(0x10,&inregs,&outregs);
}


void Pset(int x,int y,int color)
{
	int offset;

	offset = (y*320) + x;
	VGA_buffer[offset] = color;
}


void Toufu(int x,int y,int xsize, int ysize,int color)
{
	int i,j;

	for(i=0;i<ysize;i++) {
		for(j=0;j<xsize;j++) {
			Pset(x+j,y+i,color);
		}
	}

}


void DDraw(int x,int y,unsigned char *ballpointer)
{
	int i,j;
	int xsize,ysize;
	unsigned char color;

	xsize = (int) *ballpointer;
	ysize = (int) *(ballpointer + 1);
 

	for(i=0;i<ysize;i++) {
		for(j=0;j<xsize;j++) {
			color = *(ballpointer + 2 + j + i*xsize);
			if(color != 0) {
			Pset(x+j,y+i,color);
			}
		}
	}

}


void Delete(int x,int y,unsigned char *ballpointer)
{
	int i,j;
	int xsize,ysize;
	unsigned char color;

	xsize = (int) *ballpointer;
	ysize = (int) *(ballpointer + 1);
 

	for(i=0;i<ysize;i++) {
		for(j=0;j<xsize;j++) {
			color = *(ballpointer + 2 + j + i*xsize);
			if(color != 0) {
			Pset(x+j,y+i,0);
			}
		}
	}

}

void VsyncWait(void)
{
 while((inp(0x03da) & 0x08) == 0) ;
 while((inp(0x03da) & 0x08) != 0) ;
}

void Wait(int t) {
	int i=0;
	
	while(1) {
		VsyncWait();
		i++;
		if((i/70) == t) break;
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


void BDDraw (int x,int y,int width,int height,int status) {
	switch (status) {
		case 0:
		break;
		case 1:
		Toufu(x,y,width,height,15);
		break;
		case 2:
		Toufu(x,y,width,height,44);
		break;
		case 3:
		Toufu(x,y,width,height,42);
		break;
		case 4:
		Toufu(x,y,width,height,4);
		break;
		case 5:
		Toufu(x,y,width,height,3);
		break;
		case 6:
		Toufu(x,y,width,height,0);
		break;
	}
}

void Line(int x1,int y1,int x2,int y2,int color)  {

	int m;  
	int i;
	int temp;
      
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	if (x2 == x1) {
          	for (i = y1; i <= y2; i++)              
			Pset(x1, i, color);
	} else {          
		m = (y2 - y1) / (x2 - x1);
			for (i = x1; i <= x2; i++)
				Pset(i, m * (i - x1) + y1, color);      
	}  
}


void Rect(int x1,int y1,int x3,int y3,int color) {
	Line(x1,y1,x3,y1,color);
	Line(x1,y1,x1,y3,color);
	Line(x1,y3,x3,y3,color);
	Line(x3,y1,x3,y3,color);
}


void Change_Block(void) {
	int i,j;
	/* Block initialize */
	for(i=0;i<5;i++) {
		for(j=0;j<8;j++) {
			block[i*8+j].x= 20 + 35*j;
			block[i*8+j].y= 30 + 11*i;
			block[i*8+j].width= 28;
			block[i*8+j].height= 7;
			block[i*8+j].status= stg[stg_i][i*8+j];
		}
	}
	status.block_total = stg[stg_i][40];
	
	/* Status and Display initialize */
	Status(1);
	Status(2);
	Rect(0,11,319,199,14);
	Rect(160,0,275,9,4);

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


void Status(int index) {
	switch (index) {
		case 1: /* Score */
			Toufu(280,0,30,10,0);
			if((status.score-status.score_count*100 >= status.score_count) && status.score != 0) {status.life++;status.score_count++;Status(2);}
			Num(280,0,status.score);
			break;

		case 2: /* Ball Life */
			if(status.life == 0) {
				gotoxy(1,1);printf("GAME OVER");
				Wait(1);
				
					gotoxy(1,1);printf("CONTINUE? (Y/N)");
				while(1) {
					if(KeyTable[0x15] == 1) {blcflag = 3;gotoxy(1,1);printf("               ");return;}	/* CONTINUE */
					else if(KeyTable[0x31] == 1) {blcflag = 4;gotoxy(1,1);printf("               ");return;} /* EXIT */
					else ;
				}
				
			}
			Toufu(165,2,110,6,0);
			for(index=1;index<status.life;index++) {
				DDraw(165+(index-1)*10,2,ball_pic);
			}		
			break;

		case 3: /* Block Check */
			if(status.block_total == 0) {blcflag = 2;status.life++;Status(2);}	/* STAGE CLEAR */
			break;
	}
}


void Standby(void) {
   int i;
   ball.x = 171;
   ball.y = 169;
   ball.dx = 2;
   ball.dy = 2;
   racket.x = 159;



 while(1) {	/* Free Adjustment */
  VsyncWait();	
  Delete(ball.x,ball.y,ball_pic);
  Delete(racket.x,racket.y,racket_pic);

	 	/* call inkey to get keyboard */
	 if(KeyTable[0x4d] == 1) {racket.x += 4;ball.x += 4;}
	 if(KeyTable[0x4b] == 1) {racket.x -= 4;ball.x -= 4;}
	 if(KeyTable[0x39] == 1) break;
	 if(racket.x <= 1) {racket.x = 1;ball.x = racket.x + 12;}
	 if(racket.x + racket.width >= 319) {racket.x = 319 - racket.width;ball.x = racket.x + 12;}

  DDraw(racket.x,racket.y,racket_pic);
  DDraw(ball.x,ball.y,ball_pic);
	for(i=0;i<40;i++) {
		BDDraw(block[i].x,block[i].y,block[i].width,block[i].height,block[i].status);
	}
 }

}





/*******************************************************************
			   Stage Control
*******************************************************************/


void Stage(void) {
 int i,j;

_STAGEUP_:		/* Stage_up-Label */
  Change_Block();
_STANDBY_:		/* Standby-Label */
  Standby();

/******** WHILE LOOP ********/
 while(1) {

  VsyncWait();
  disable();
  Delete(ball.x,ball.y,ball_pic);
  Delete(racket.x,racket.y,racket_pic);
  enable();

/******** RACKET CHECK ********/
	 if(KeyTable[0x4d] == 1) racket.x += 4;
	 if(KeyTable[0x4b] == 1) racket.x -= 4;
	 if(KeyTable[0x01] == 1) break;
	 if(racket.x <= 1) racket.x = 1;
	 if(racket.x + racket.width >= 319) racket.x = 319 - racket.width;

/******** BALL POSITION CHECK ********/
	ball.x += ball.dx;
	ball.y += ball.dy;
	if(ball.x <= 1) {ball.x = 1; ball.dx = -ball.dx;}
	if(ball.x + ball.width >= 319) {ball.x = 319 - ball.width; ball.dx = -ball.dx;}
	if(ball.y <= 12) {ball.y = 12; ball.dy = -ball.dy;}
	if(ball.y + ball.height >= 200) {
		status.life--;
		Status(2);
		Wait(1);
		if(blcflag == 3) {status.life = 3;status.score = 0;goto _STAGEUP_;}
		else if(blcflag == 4) return;
		goto _STANDBY_;
	}

/******** HIT CHECK RACKET AND BALL ********/
	if((ball.y + ball.height >= racket.y) && (ball.y <= racket.y + racket.height/2)) {
		if((ball.x >= racket.x - ball.width) && (ball.x <= racket.x + racket.width + 2)) {
			ball.y = racket.y - ball.height;
			ball.dy = -ball.dy;
			ballRD = ball.x - racket.x;
			if(ballRD < 0) {ball.dx = -4;}
			if(ballRD > 30) {ball.dx = 4;}
			if((ballRD <= 4) && (ballRD >= 0)) {ball.dx = -3;}
			if((ballRD >= 26) && (ballRD <= 30)) {ball.dx = 3;}
			if((ballRD > 4) && (ballRD < 12)) {ball.dx = -2;}
			if((ballRD > 18) && (ballRD < 26)) {ball.dx = 2;}
			if((ballRD > 15) && (ballRD <= 18)) {ball.dx = 1;}
			if((ballRD >= 12) && (ballRD <= 15)) {ball.dx = -1;}
		}
	}

/******** HIT CHECK BLOCK AND BALL ********/
	blcflag=0;
	tempX = ball.x + 3;
	tempY = ball.y + 3;
	if(tempX > 17){if(tempX < 297){if(tempY > 25){if(tempY < 83){
		for(i=0;i<40;i++){
		if(blcflag == 1) break;
			if((tempX >= block[i].x) && (tempX <= block[i].x + block[i].width)) {
				for(j=-1;j<=1;j++) {
					if(tempY == block[i].y + block[i].height + j) {
						if(block[i].status > 0) {
							if(ball.dy < 0 ) {
								ball.dy = -ball.dy;
								block[i].status--;
								if(block[i].status == 0) {status.block_total--;Toufu(block[i].x,block[i].y,block[i].width,block[i].height,0);}
								blcflag = 1;status.score+=1;Status(1);
								break;
							}
						}
						
					}else if(tempY == block[i].y + j) {
						if(block[i].status > 0) {
							if(ball.dy > 0 ) {
								ball.dy = -ball.dy;
								block[i].status--;
								if(block[i].status == 0) {status.block_total--;Toufu(block[i].x,block[i].y,block[i].width,block[i].height,0);}
								blcflag = 1;status.score+=2;Status(1);
								break;
							}
						}
					}
				}
			} else if((tempY >= block[i].y) && (tempY <= block[i].y + block[i].height)) {
				for(j=-3;j<=3;j++) {
					if(tempX == block[i].x + j) {
						if(block[i].status > 0) {
							if(ball.dx > 0 ) {
								ball.dx = -ball.dx;
								block[i].status--;
								if(block[i].status == 0) {status.block_total--;Toufu(block[i].x,block[i].y,block[i].width,block[i].height,0);}
								blcflag = 1;status.score+=5;Status(1);
								break;
							}
						}	
					} else if(tempX == block[i].x + block[i].width + j) {
						if(block[i].status > 0) {
							if(ball.dx < 0 ) {
								ball.dx = -ball.dx;
								block[i].status--;
								if(block[i].status == 0) {status.block_total--;Toufu(block[i].x,block[i].y,block[i].width,block[i].height,0);}
								blcflag = 1;status.score+=5;Status(1);
								break;
							}
						}
					}
				}
			}
		}
}}}}
/******** TOTAL CHECK ********/

  Status(3);
  if(blcflag == 2) {Wait(2);stg_i++;goto _STAGEUP_;}
/******** DRAW PICTURE AND STATUS ********/
  disable();
  DDraw(racket.x,racket.y,racket_pic);
  DDraw(ball.x,ball.y,ball_pic);
	for(i=0;i<40;i++) {
		BDDraw(block[i].x,block[i].y,block[i].width,block[i].height,block[i].status);
	}
  enable();
/******** END OF WHILE LOOP ********/
 } 
}