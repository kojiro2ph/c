void VGA13h(void);
void VGA03h(void);
void Pset(int x,int y,int color);
void Toufu(int x,int y,int xsize, int ysize,int color);
void DDraw(int x,int y,unsigned char *ballpointer);
void Delete(int x,int y,unsigned char *ballpointer);
void VsyncWait(void);

unsigned char far *VGA_buffer = MK_FP(0x0A000,0);

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
			if((x+j >= 0)&&(x+j <= 319)&&(y+i >= 0)&&(y+i <= 199))
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
			if((x+j >= 0)&&(x+j <= 319)&&(y+i >= 0)&&(y+i <= 199)) {
				color = *(ballpointer + 2 + j + i*xsize);
				if(color != 0) {
					Pset(x+j,y+i,color);
				}
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